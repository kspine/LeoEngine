/*
\par �޸�ʱ��:
2017-03-14 00:06 +0800
*/

#include "LSchemeA.h"
#include "SContext.h"

using namespace leo;

namespace scheme
{

	ValueNode
		MapLSLALeafNode(const TermNode& term)
	{
		return AsNode(string(),
			string(Deliteralize(ParseLSLANodeString(MapToValueNode(term)))));
	}

	ValueNode
		TransformToSyntaxNode(const ValueNode& node, const string& name)
	{
		ValueNode::Container con{ AsIndexNode(size_t(), node.GetName()) };
		const auto nested_call([&](const ValueNode& nd) {
			con.emplace(TransformToSyntaxNode(nd, MakeIndex(con)));
		});

		if (node.empty())
		{
			if (const auto p = AccessPtr<NodeSequence>(node))
				for (auto& nd : *p)
					nested_call(nd);
			else
				con.emplace(NoContainer, MakeIndex(1),
					Literalize(ParseLSLANodeString(node)));
		}
		else
			for (auto& nd : node)
				nested_call(nd);
		return{ std::move(con), name };
	}

	string
		EscapeNodeLiteral(const ValueNode& node)
	{
		return EscapeLiteral(Access<string>(node));
	}

	string
		LiteralizeEscapeNodeLiteral(const ValueNode& node)
	{
		return Literalize(EscapeNodeLiteral(node));
	}

	string
		ParseLSLANodeString(const ValueNode& node)
	{
		return leo::value_or(AccessPtr<string>(node));
	}


	string
		DefaultGenerateIndent(size_t n)
	{
		return string(n, '\t');
	}

	void
		PrintIndent(std::ostream& os, IndentGenerator igen, size_t n)
	{
		if (LB_LIKELY(n != 0))
			leo::write(os, igen(n));
	}

	void
		PrintNode(std::ostream& os, const ValueNode& node, NodeToString node_to_str,
			IndentGenerator igen, size_t depth)
	{
		PrintIndent(os, igen, depth);
		os << EscapeLiteral(node.GetName()) << ' ';
		if (PrintNodeString(os, node, node_to_str))
			return;
		os << '\n';
		if (node)
		{
			const auto nested_call(std::bind(PrintNodeChild, std::ref(os),
				std::placeholders::_1, node_to_str, igen, depth));

			// TODO: Null coalescing or variant value?
			if (const auto p = AccessPtr<NodeSequence>(node))
				for (const auto& nd : *p)
					nested_call(nd);
			else
				for (const auto& nd : node)
					nested_call(nd);
		}
	}

	void
		PrintNodeChild(std::ostream& os, const ValueNode& node,
			NodeToString node_to_str, IndentGenerator igen, size_t depth)
	{
		PrintIndent(os, igen, depth);
		if (IsPrefixedIndex(node.GetName()))
			PrintNodeString(os, node, node_to_str);
		else
		{
			os << '(' << '\n';
			TryExpr(PrintNode(os, node, node_to_str, igen, depth + 1))
				CatchIgnore(std::out_of_range&)
				PrintIndent(os, igen, depth);
			os << ')' << '\n';
		}
	}

	bool
		PrintNodeString(std::ostream& os, const ValueNode& node,
			NodeToString node_to_str)
	{
		TryRet(os << node_to_str(node) << '\n', true)
			CatchIgnore(leo::bad_any_cast&)
			return{};
	}


	namespace sxml
	{

		string
			ConvertAttributeNodeString(const TermNode& term)
		{
			switch (term.size())
			{
			default:
				TraceDe(Warning, "Invalid term with more than 2 children found.");
			case 2:
			{
				auto i(term.begin());
				const auto& n(Access<string>(Deref(i)));

				return n + '=' + Access<string>(Deref(++i));
			}
			case 1:
				return Access<string>(Deref(term.begin()));
			case 0:
				break;
			}
			throw LoggedEvent("Invalid term with less than 1 children found.", Warning);
		}

		string
			ConvertDocumentNode(const TermNode& term, IndentGenerator igen, size_t depth,
				ParseOption opt)
		{
			if (term)
			{
				string res(ConvertStringNode(term));

				if (res.empty())
				{
					if (opt == ParseOption::String)
						throw LoggedEvent("Invalid non-string term found.");

					const auto& con(term.GetContainer());

					if (!con.empty())
						try
					{
						auto i(con.cbegin());
						const auto& str(Access<string>(Deref(i)));

						++i;
						if (str == "@")
						{
							for (; i != con.cend(); ++i)
								res += ' ' + ConvertAttributeNodeString(Deref(i));
							return res;
						}
						if (opt == ParseOption::Attribute)
							throw LoggedEvent("Invalid non-attribute term found.");
						if (str == "*PI*")
						{
							res = "<?";
							for (; i != con.cend(); ++i)
								res += string(Deliteralize(ConvertDocumentNode(
									Deref(i), igen, depth, ParseOption::String)))
								+ ' ';
							if (!res.empty())
								res.pop_back();
							return res + "?>";
						}
						if (str == "*ENTITY*" || str == "*NAMESPACES*")
						{
							if (opt == ParseOption::Strict)
								throw leo::unimplemented();
						}
						else if (str == "*COMMENT*")
							;
						else if (!str.empty())
						{
							const bool is_content(str != "*TOP*");
							string head('<' + str);
							bool nl{};

							if (LB_UNLIKELY(!is_content && depth > 0))
								TraceDe(Warning, "Invalid *TOP* found.");
							if (i != con.end())
							{
								if (!Deref(i).empty()
									&& (i->begin())->Value == string("@"))
								{
									head += string(
										Deliteralize(ConvertDocumentNode(*i, igen,
											depth, ParseOption::Attribute)));
									if (++i == con.cend())
										return head + " />";
								}
								head += '>';
							}
							else
								return head + " />";
							for (; i != con.cend(); ++i)
							{
								nl = Deref(i).Value.GetType()
									!= leo::type_id<string>();
								if (nl)
									res += '\n' + igen(depth + size_t(is_content));
								else
									res += ' ';
								res += string(Deliteralize(ConvertDocumentNode(*i,
									igen, depth + size_t(is_content))));
							}
							if (res.size() > 1 && res.front() == ' ')
								res.erase(0, 1);
							if (!res.empty() && res.back() == ' ')
								res.pop_back();
							if (is_content)
							{
								if (nl)
									res += '\n' + igen(depth);
								return head + res + leo::quote(str, "</", '>');
							}
						}
						else
							throw LoggedEvent("Empty item found.", Warning);
					}
					CatchExpr(leo::bad_any_cast& e, TraceDe(Warning,
						"Conversion failed: <%s> to <%s>.", e.from(), e.to()))
				}
				return res;
			}
			throw LoggedEvent("Empty term found.", Warning);
		}

		string
			ConvertStringNode(const TermNode& term)
		{
			return leo::call_value_or(EscapeXML, AccessPtr<string>(term));
		}

		void
			PrintSyntaxNode(std::ostream& os, const TermNode& term, IndentGenerator igen,
				size_t depth)
		{
			if (IsBranch(term))
				leo::write(os,
					ConvertDocumentNode(Deref(term.begin()), igen, depth), 1);
			os << std::flush;
		}


		ValueNode
			MakeXMLDecl(const string& name, const string& ver, const string& enc,
				const string& sd)
		{
			auto decl("version=\"" + ver + '"');

			if (!enc.empty())
				decl += " encoding=\"" + enc + '"';
			if (!sd.empty())
				decl += " standalone=\"" + sd + '"';
			return AsNodeLiteral(name, { { MakeIndex(0), "*PI*" },{ MakeIndex(1), "xml" },
			{ MakeIndex(2), decl + ' ' } });
		}

		ValueNode
			MakeXMLDoc(const string& name, const string& ver, const string& enc,
				const string& sd)
		{
			auto doc(MakeTop(name));

			doc.Add(MakeXMLDecl(MakeIndex(1), ver, enc, sd));
			return doc;
		}

	} // namespace sxml;


	namespace
	{

		string
			InitBadIdentifierExceptionString(string&& id, size_t n)
		{
			return (n != 0 ? (n == 1 ? "Bad identifier: '" : "Duplicate identifier: '")
				: "Unknown identifier: '") + std::move(id) + "'.";
		}

	} // unnamed namespace;


	ImplDeDtor(LSLException)


		ImplDeDtor(ListReductionFailure)


		ImplDeDtor(InvalidSyntax)

		ImplDeDtor(ParameterMismatch)

		ArityMismatch::ArityMismatch(size_t e, size_t r, RecordLevel lv)
		: LSLException(leo::sfmt("Arity mismatch: expected %zu, received %zu.",
			e, r), lv),
		expected(e), received(r)
	{}
	ImplDeDtor(ArityMismatch)

		BadIdentifier::BadIdentifier(const char* id, size_t n, RecordLevel lv)
		: LSLException(InitBadIdentifierExceptionString(id, n), lv),
		p_identifier(make_shared<string>(id))
	{}
	BadIdentifier::BadIdentifier(string_view id, size_t n, RecordLevel lv)
		: LSLException(InitBadIdentifierExceptionString(string(id), n), lv),
		p_identifier(make_shared<string>(id))
	{}
	ImplDeDtor(BadIdentifier)


		LexemeCategory
		CategorizeBasicLexeme(string_view id) lnothrowv
	{
		LAssertNonnull(id.data() && !id.empty());

		const auto c(CheckLiteral(id));

		if (c == '\'')
			return LexemeCategory::Code;
		if (c != char())
			return LexemeCategory::Data;
		return LexemeCategory::Symbol;
	}

	LexemeCategory
		CategorizeLexeme(string_view id) lnothrowv
	{
		const auto res(CategorizeBasicLexeme(id));

		return res == LexemeCategory::Symbol && IsLSLAExtendedLiteral(id)
			? LexemeCategory::Extended : res;
	}

	bool
		IsLSLAExtendedLiteral(string_view id) lnothrowv
	{
		LAssertNonnull(id.data() && !id.empty());

		const char f(id.front());

		return (id.size() > 1 && IsLSLAExtendedLiteralNonDigitPrefix(f)
			&& id.find_first_not_of("+-") != string_view::npos)
			|| std::isdigit(f);
	}

	observer_ptr<const string>
		TermToNamePtr(const TermNode& term)
	{
		return AccessPtr<TokenValue>(term);
	}


	void
		TokenizeTerm(TermNode& term)
	{
		for (auto& child : term)
			TokenizeTerm(child);
		if (const auto p = AccessPtr<string>(term))
			term.Value.emplace<TokenValue>(std::move(*p));
	}


	ValueObject
		ReferenceValue(const ValueObject& vo)
	{
		if (vo)
		{
			if (!vo.OwnsUnique())
				return vo.MakeIndirect();
			else
				throw LSLException("Value of a temporary shall not be referenced.");
		}
		else
			leo::throw_invalid_construction();
	}

	void
		DefineValue(ContextNode& ctx, string_view id, ValueObject&& vo, bool forced)
	{
		LAssertNonnull(id.data());
		if (forced)
			// XXX: Self overwriting is possible.
			ctx[id].Value = std::move(vo);
		else if (!ctx.AddValue(id, std::move(vo)))
			throw BadIdentifier(id, 2);
	}

	void
		RedefineValue(ContextNode& ctx, string_view id, ValueObject&& vo, bool forced)
	{
		LAssertNonnull(id.data());
		if (const auto p = AccessNodePtr(ctx, id))
			p->Value = std::move(vo);
		else if (!forced)
			throw BadIdentifier(id, 0);
	}

	void
		RemoveIdentifier(ContextNode& ctx, string_view id, bool forced)
	{
		LAssertNonnull(id.data());
		if (!ctx.Remove(id) && !forced)
			throw BadIdentifier(id, 0);
	}


	bool
		CheckReducible(ReductionStatus status)
	{
		if (status == ReductionStatus::Clean
			|| status == ReductionStatus::Retained)
			return {};
		if (LB_UNLIKELY(status != ReductionStatus::Retrying))
			TraceDe(Warning, "Unexpected status found");
		return true;
	}

	ReductionStatus
		ReduceHeadEmptyList(TermNode& term) lnothrow
	{
		if (term.size() > 1 && IsEmpty(Deref(term.begin())))
			RemoveHead(term);
		return ReductionStatus::Clean;
	}

	ReductionStatus
		ReduceToList(TermNode& term) lnothrow
	{
		return IsBranch(term) ? (void(RemoveHead(term)), ReductionStatus::Retained)
			: ReductionStatus::Clean;
	}
	
} // namespace scheme;
