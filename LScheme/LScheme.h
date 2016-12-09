#ifndef LScheme_LScheme_H
#define LScheme_LScheme_H 1

#include "LSchemeA.h"

namespace scheme {
	namespace v1 {
		/*!
		\brief LSLV1 Ԫ��ǩ��
		\note LSLV1 �� LSLA �ľ���ʵ�֡�
		*/
		struct LS_API LSLV1Tag : LSLATag
		{};


		//! \brief ֵ�Ǻţ��ڵ��е�ֵ��ռλ����
		enum class ValueToken
		{
			Null,
			/*!
			\brief δ����ֵ��
			*/
			Undefined,
			/*!
			\brief δָ��ֵ��
			*/
			Unspecified,
			GroupingAnchor,
			OrderedAnchor
		};


		//@{
		//! \brief ���� LSLV1 �ӽڵ㡣
		//@{
		/*!
		\note �������������˳��

		��һ����ָ���ı任�������ڶ�����ָ����������
		��ӳ��������ؽڵ�����Ϊ������ݵ�ǰ�������ӽڵ�������ǰ׺ $ �����Ա����ظ���
		*/
		LS_API void
			InsertChild(TermNode&&, TermNode::Container&);

		/*!
		\note ����˳��

		ֱ�Ӳ��� LSLV1 �ӽڵ㵽��������ĩβ��
		*/
		LS_API void
			InsertSequenceChild(TermNode&&, NodeSequence&);
		//@}

		/*!
		\brief �任 LSLV1 �ڵ� S ���ʽ�����﷨��Ϊ LSLV1 ����ṹ��
		\exception std::bad_function_call �����������Ϊ�ա�
		\return �任����½ڵ㣨���ӽڵ㣩��

		��һ����ָ��Դ�ڵ㣬�������ָ�����ֱ任����
		�������õĵڶ������Ĳ������޸Ĵ���Ľڵ����ʱ�任���޸�Դ�ڵ㡣
		�������£�
		��Դ�ڵ�ΪҶ�ڵ㣬ֱ�ӷ���ʹ�õ�����������ӳ��Ľڵ㡣
		��Դ�ڵ�ֻ��һ���ӽڵ㣬ֱ�ӷ�������ӽڵ�ı任�����
		����ʹ�õ��Ĳ����ӵ�һ���ӽڵ�ȡ��Ϊ�任������Ƶ��ַ�����
		�����Ʒǿ�����Ե�һ���ӽڵ㣬ֻ�任ʣ���ӽڵ㡣
		��ʣ��һ���ӽڵ㣨��Դ�ڵ��������ӽڵ㣩ʱ��ֱ�ӵݹ�任����ڵ㲢���ء�
		���任��Ľ�����Ʒǿգ�����Ϊ�����ֵ�����򣬽����Ϊ�����ڵ�һ��ֵ��
		�����½��ڵ��������������任ʣ��Ľڵ������������������������������Ľڵ㡣
		�ڶ�����ָ����ʱ��ӳ���������Ϊ����Ĭ��ʹ�õݹ� TransformNode ���á�
		���õ����������ӳ��Ľ����������
		*/
		LS_API ValueNode
			TransformNode(const TermNode&, NodeMapper = {}, NodeMapper = MapLSLALeafNode,
				NodeToString = ParseLSLANodeString, NodeInserter = InsertChild);

		/*!
		\brief �任 LSLV1 �ڵ� S ���ʽ�����﷨��Ϊ LSLV1 ��������ṹ��
		\exception std::bad_function_call �����������Ϊ�ա�
		\return �任����½ڵ㣨���ӽڵ㣩��
		\sa TransformNode

		�� TransformNode �任������ͬ��
		��������ӽڵ��� NodeSequence ����ʽ��Ϊ�任�ڵ��ֵ�������ӽڵ㣬�ɱ���˳��
		*/
		LS_API ValueNode
			TransformNodeSequence(const TermNode&, NodeMapper = {},
				NodeMapper = MapLSLALeafNode, NodeToString = ParseLSLANodeString,
				NodeSequenceInserter = InsertSequenceChild);
		//@}


		/*!
		\brief ���� LSLV1 ���뵥Ԫ��
		\throw LoggedEvent ���棺�����������е�ʵ��ת��ʧ�ܡ�
		*/
		//@{
		template<typename _type, typename... _tParams>
		ValueNode
			LoadNode(_type&& tree, _tParams&&... args)
		{
			TryRet(v1::TransformNode(std::forward<TermNode&&>(tree),
				lforward(args)...))
				CatchThrow(leo::bad_any_cast& e, LoggedEvent(leo::sfmt(
					"Bad LSLV1 tree found: cast failed from [%s] to [%s] .", e.from(),
					e.to()), leo::Warning))
		}

		template<typename _type, typename... _tParams>
		ValueNode
			LoadNodeSequence(_type&& tree, _tParams&&... args)
		{
			TryRet(v1::TransformNodeSequence(std::forward<TermNode&&>(tree),
				lforward(args)...))
				CatchThrow(leo::bad_any_cast& e, LoggedEvent(leo::sfmt(
					"Bad LSLV1 tree found: cast failed from [%s] to [%s] .", e.from(),
					e.to()), leo::Warning))
		}
		//@}


		//@{
		//! \brief �����ػ��顣
		LS_API GuardPasses&
			AccessGuardPassesRef(ContextNode&);

		//! \brief ����Ҷ�ڵ�顣
		LS_API EvaluationPasses&
			AccessLeafPassesRef(ContextNode&);

		//! \brief �����б�ڵ�顣
		LS_API EvaluationPasses&
			AccessListPassesRef(ContextNode&);
		//@}

		/*!
		\brief �����������顣
		*/
		LS_API LiteralPasses&
			AccessLiteralPassesRef(ContextNode&);

		//! \sa InvokePasses
		//@{
		/*!
		\brief �����ػ��顣
		\sa GuardPasses
		*/
		LS_API Guard
			InvokeGuard(TermNode& term, ContextNode&);

		/*!
		\sa EvaluationPasses
		*/
		//@{
		//! \brief ����Ҷ�ڵ�顣
		LS_API ReductionStatus
			InvokeLeaf(TermNode& term, ContextNode&);

		//! \brief �����б�ڵ�顣
		LS_API ReductionStatus
			InvokeList(TermNode& term, ContextNode&);
		//@}

		/*!
		\brief �����������顣
		\pre ���ԣ��ַ�������������ָ��ǿա�
		\sa LiteralPasses
		*/
		LS_API ReductionStatus
			InvokeLiteral(TermNode& term, ContextNode&, string_view);
		//@}


		/*!
		\brief LSLV1 ���ʽ�ڵ��Լ����������һ����ֵ���̹�Լ�ӱ��ʽ��
		\return ��Լ״̬��
		\note ����ʹ�����������ĵ�����ʧЧ��
		\note Ĭ�ϲ���Ҫ�ع�Լ����ɱ���ֵ��ı䡣
		\note �ɱ���ֵ�������ʵ�ֵݹ���ֵ��
		\note �쳣��ȫȡ���ڵ��ñ������쳣��ȫ��֤��
		\sa DetectReducible
		\sa InvokeGuard
		\sa InvokeLeaf
		\sa InvokeList
		\sa ValueToken
		\todo ʵ�� ValueToken ��������

		��Լ˳�����£�
		���� InvokeGuard ���б�Ҫ�����������ã�
		������Լ��ֱ������Ҫ�����ع�Լ��
		��Ӧ��ͬ�Ľڵ�μ��ṹ���࣬һ�ε���������˳���ж�ѡ�����·�֧֮һ�������Լ���
		��֦�ڵ���� InvokeList ��ֵ��
		�Կսڵ��滻Ϊ ValueToken::Null ��
		�����滻Ϊ ValueToken ��Ҷ�ڵ㱣������
		������Ҷ�ڵ���� InvokeLeaf ��ֵ��
		��һ��ֵ�Ľ����Ϊ DetectReducible �ĵڶ����������ݽ���ж��Ƿ�����ع�Լ��
		�˴�Լ���ĵ����жԽڵ�ľ���ṹ����Ĭ��Ҳ���������� LSLV1 ʵ�� API ��
		�������Ӧ����ָ����ȷ��˳��
		���������������ڵ㲻�Ǳ��ʽ����ṹ�����ǳ����﷨������ API ���� TransformNode ��
		*/
		LS_API ReductionStatus
			Reduce(TermNode&, ContextNode&);

		/*!
		\note �����Թ淶�������Լ˳��δָ����
		\note ����ʹ�����������ĵ�����ʧЧ��
		\sa Reduce
		*/
		//@{
		//! \note ���������ع�ԼҪ��
		//@{
		/*!
		\brief �������еĵڶ��ʼ�����Լ��
		\throw InvalidSyntax �����ڵ������������� 1 ��
		\sa ReduceChildren
		*/
		LS_API void
			ReduceArguments(TermNode::Container&, ContextNode&);

		/*!
		\todo ʹ�ø�ȷ�е��쳣���͡�
		*/
		//@{
		/*!
		\brief ��Լ�����ɹ������� Reduce ���������ʧ��ʱ�׳��쳣��
		\throw LSLException Reduce ������� ReductionStatus::Success��
		\sa CheckedReduceWith
		\sa Reduce
		*/
		LS_API void
			ReduceChecked(TermNode&, ContextNode&);

		/*!
		\brief ��Լ�հ���ʹ�õ��Ĳ���ָ���ıհ����Լ���滻��ָ�����ϡ�
		\exception LSLException �쳣�������� ReduceChecked �׳���
		\note ��������ָ���Ƿ�ת�ƶ�������ԭ�
		\sa ReduceChecked
		*/
		LS_API void
			ReduceCheckedClosure(TermNode&, ContextNode&, bool, TermNode&);
		//@}

		/*!
		\brief ��Լ���
		*/
		//@{
		LS_API void
			ReduceChildren(TNIter, TNIter, ContextNode&);
		inline PDefH(void, ReduceChildren, TermNode::Container& con, ContextNode& ctx)
			ImplExpr(ReduceChildren(con.begin(), con.end(), ctx))
			inline PDefH(void, ReduceChildren, TermNode& term, ContextNode& ctx)
			ImplExpr(ReduceChildren(term.GetContainerRef(), ctx))
			//@}
			//@}

			/*!
			\brief �Ƴ��������ָ��������������Լ��
			*/
			LS_API ReductionStatus
			ReduceTail(TermNode&, ContextNode&, TNIter);
		//@}

		/*!
		\brief ��Լ���
		\return ��Լ״̬��
		\sa Reduce
		\see https://en.wikipedia.org/wiki/Fexpr ��

		�����ϸ��Է�������������ֵ֦�ڵ��һ���Ա����ȷ�����ƶ��ӱ��ʽ��ֵ�ĸ��Ӹ��Ӷȡ�
		*/
		inline PDefH(ReductionStatus, ReduceFirst, TermNode& term, ContextNode& ctx)
			ImplRet(IsBranch(term) ? Reduce(Deref(term.begin()), ctx)
				: ReductionStatus::Success)


			/*!
			\brief ���ø�����Ƚڵ㣺���ù�Լʱ��ʾ��Ⱥ������ĵ���Ϣ��
			\note ��Ҫ���ڵ��ԡ�
			\sa InvokeGuard
			*/
			LS_API void
			SetupTraceDepth(ContextNode& ctx, const string& name = limpl("$__depth"));


		/*!
		\note ValueObject �����ֱ�ָ���滻��ӵ�ǰ׺�ͱ��滻�ķָ�����ֵ��
		*/
		//@{
		/*!
		\note �Ƴ�������ֵ��ָ���ָ���ָ��������� AsIndexNode ���ָ��ǰ׺ֵ��Ϊ���
		\note ���һ������ָ������ֵ�����ơ�
		\sa AsIndexNode
		*/
		//@{
		//! \brief �任�ָ�����׺���ʽΪǰ׺���ʽ��
		LS_API TermNode
			TransformForSeparator(const TermNode&, const ValueObject&, const ValueObject&,
				const string& = {});

		//! \brief �ݹ�任�ָ�����׺���ʽΪǰ׺���ʽ��
		LS_API TermNode
			TransformForSeparatorRecursive(const TermNode&, const ValueObject&,
				const ValueObject&, const string& = {});
		//@}

		/*!
		\brief �������е�ָ���ָ��������ҵ����滻��Ϊȥ���ָ���������滻ǰ׺����ʽ��
		\return �Ƿ��ҵ����滻���
		\sa EvaluationPasses
		\sa TransformForSeparator
		*/
		LS_API ReductionStatus
			ReplaceSeparatedChildren(TermNode&, const ValueObject&, const ValueObject&);
		//@}


		/*!
		\brief ��ʽ�����Ĵ�������
		*/
		class LS_API FormContextHandler
		{
		public:
			ContextHandler Handler;
			/*!
			\brief �������̣���֤����װ�Ĵ������ĵ��÷���ǰ��������
			*/
			std::function<bool(const TermNode&)> Check{};

			template<typename _func,
				limpl(typename = leo::exclude_self_t<FormContextHandler, _func>)>
				FormContextHandler(_func&& f)
				: Handler(lforward(f))
			{}
			template<typename _func, typename _fCheck>
			FormContextHandler(_func&& f, _fCheck c)
				: Handler(lforward(f)), Check(c)
			{}

			/*!
			\brief ����һ����ʽ��
			\exception LSLException �쳣������
			\throw LoggedEvent ���棺���Ͳ�ƥ�䣬
			�� Handler �׳��� leo::bad_any_cast ת����
			\throw LoggedEvent ������ Handler �׳��� leo::bad_any_cast ���
			std::exception ת����
			\throw std::invalid_argument ����δͨ����

			���鲻���ڻ��ڼ��ͨ���󣬶Խڵ���� Hanlder �������׳��쳣��
			*/
			void
				operator()(TermNode&, ContextNode&) const;

			/*!
			\brief �Ƚ������Ĵ�������ȡ�
			\note ���Լ�����̵ĵȼ��ԡ�
			*/
			friend PDefHOp(bool, == , const FormContextHandler& x,
				const FormContextHandler& y)
				ImplRet(x.Handler == y.Handler)
		};


		/*!
		\brief ���������Ĵ�������
		\since build 696
		*/
		class LS_API FunctionContextHandler
		{
		public:
			FormContextHandler Handler;

			template<typename _func,
				limpl(typename = leo::exclude_self_t<FunctionContextHandler, _func>)>
				FunctionContextHandler(_func&& f)
				: Handler(lforward(f))
			{}
			template<typename _func, typename _fCheck>
			FunctionContextHandler(_func&& f, _fCheck c)
				: Handler(lforward(f), c)
			{}

			/*!
			\brief ��������
			\throw ListReductionFailure �б��������һ�
			\sa ReduceArguments

			��ÿһ��������ֵ��Ȼ�����������Կɵ��õ������ Hanlder �������׳��쳣��
			*/
			void
				operator()(TermNode&, ContextNode&) const;

			friend PDefHOp(bool, == , const FunctionContextHandler& x,
				const FunctionContextHandler& y)
				ImplRet(x.Handler == y.Handler)
		};


		//@{
		template<typename... _tParams>
		inline void
			RegisterFormContextHandler(ContextNode& node, const string& name,
				_tParams&&... args)
		{
			scheme::RegisterContextHandler(node, name,
				FormContextHandler(lforward(args)...));
		}

		//! \brief ת�������Ĵ�������
		template<typename... _tParams>
		inline ContextHandler
			ToContextHandler(_tParams&&... args)
		{
			return FunctionContextHandler(lforward(args)...);
		}

		/*!
		\brief ע�ắ�������Ĵ�������
		\note ʹ�� ADL ToContextHandler ��
		*/
		template<typename... _tParams>
		inline void
			RegisterFunction(ContextNode& node, const string& name, _tParams&&... args)
		{
			scheme::RegisterContextHandler(node, name,
				ToContextHandler(lforward(args)...));
		}
		//@}

		/*!
		\brief ע��ָ���ת���任�ʹ������̡�
		\sa LSL::RegisterContextHandler
		\sa ReplaceSeparatedChildren
		*/
		LS_API void
			RegisterSequenceContextTransformer(EvaluationPasses&, ContextNode&,
				const string&, const ValueObject&);


		/*!
		\brief ������������԰��������б���ֵ��
		\return ��Լ״̬��
		\throw ListReductionFailure ��Լʧ�ܣ��Ҳ����ɹ�Լ�
		\note �����֦�ڵ�����Ϊ��Լ�ɹ���û���������á�
		\sa ContextHandler
		\sa Reduce
		*/
		LS_API ReductionStatus
			EvaluateContextFirst(TermNode&, ContextNode&);

		/*!
		\exception BadIdentifier ��ʶ��δ������
		\note ��һ����ָ���������� Value ָ�������ֵ��
		\note Ĭ����Ϊ��Լ�ɹ��Ա�֤ǿ�淶�����ʡ�
		*/
		//@{
		//! \pre ���ԣ���������������ָ��ǿա�
		//@{
		/*!
		\brief ��ֵ��ʶ����
		\note ����֤��ʶ���Ƿ�Ϊ����������������������ʱ������Ҫ�ع�Լ��
		sa EvaluateTermNode
		\sa FetchValuePtr
		\sa LiftTermRef
		\sa LiteralHandler

		���ν���������ֵ������
		���� FetchValuePtr ����ֵ����ʧ���׳�δ�����쳣��
		���� LiftTermRef �滻�ڵ��ֵ��
		�� LiteralHandler ���������������������ɹ����ò������������������Ĵ�������
		��δ���أ����� EvaluateTermNode ��ֵ��
		*/
		LS_API ReductionStatus
			EvaluateIdentifier(TermNode&, const ContextNode&, string_view);

		/*!
		\brief ��ֵҶ�ڵ�Ǻš�
		\sa CategorizeLiteral
		\sa DeliteralizeUnchecked
		\sa EvaluateIdentifier
		\sa InvokeLiteral

		����ǿ��ַ�����ʾ�Ľڵ�Ǻš�
		���ν���������ֵ������
		�Դ�����������ȥ���������߽�ָ������һ����ֵ��
		��������������ȥ���������߽�ָ�����Ϊ�ַ���ֵ��
		��������������ͨ�������������鴦��
		�����ֵ���������ı�ʶ����
		*/
		LS_API ReductionStatus
			EvaluateLeafToken(TermNode&, ContextNode&, string_view);
		//@}

		/*!
		\brief ��ֵ�Խڵ����ݽṹ��ӱ�ʾ���
		\sa IsBranch
		\sa LiftTermRef

		�� TermNode �������ֵ�����ɹ����� LiftTermRef �滻ֵ��
		����������֦�ڵ㣬����Ҫ���ع�Լ��
		������ʶԹ�Լ����ת�ƵĿ���δ��ֵ�Ĳ������Ǳ�Ҫ�ġ�
		*/
		LS_API ReductionStatus
			EvaluateTermNode(TermNode&);

		/*!
		\brief ��Լ��ȡ���Ƶ�Ҷ�ڵ�Ǻš�
		\sa EvaluateLeafToken
		\sa TermToNode
		*/
		LS_API ReductionStatus
			ReduceLeafToken(TermNode&, ContextNode&);
		//@}

		/*!
		\brief ����Ĭ�Ͻ��ͣ�����ʹ�õĹ�������顣
		\note ��ǿ�쳣��ȫ������������ǰ����״̬������ʧ��ʱ�ع���
		\sa EvaluateContextFirst
		\sa ReduceFirst
		\sa ReduceLeafToken
		*/
		LS_API void
			SetupDefaultInterpretation(ContextNode&, EvaluationPasses);


		/*!
		\brief LSLV1 �﷨��ʽ��Ӧ�Ĺ���ʵ�֡�
		*/
		namespace Forms
		{

			/*!
			\pre ���ԣ����������Ӧ֦�ڵ㡣
			*/
			//@{
			//! \brief ������ӳ���ֵ��
			inline PDefH(void, Quote, const TermNode& term) lnothrowv
				ImplExpr(LAssert(IsBranch(term), "Invalid term found."))

				/*!
				\return ��Ĳ���������

				��ʹ�� RegisterFormContextHandler ע�������Ĵ��������ο��ķ���
				$quote|$quoteN <expression>
				*/
				//@{
				//! \brief ȡ��Ĳ����������������� 1 ��
				inline PDefH(size_t, FetchArgumentN, const TermNode& term) lnothrowv
				ImplRet(Quote(term), term.size() - 1)

				/*!
				\brief ���þ����ȷ������ָ��������������ӳ���ֵ��
				\throw ArityMismatch ��Ĳ������������ڵڶ�������
				*/
				LS_API size_t
				QuoteN(const TermNode&, size_t = 1);
			//@}


			/*!
			\brief ��������Ƿ����Ϊ���η��ĵڶ���������������Ƴ���
			\return �Ƿ���ڲ��Ƴ������η���

			����һ����ָ�������������Ƿ���ڵڶ�����ָ�������η�Ϊ��ĵ�һ���������������Ƴ���
			*/
			//@{
			LS_API bool
				ExtractModifier(TermNode::Container&, const ValueObject& = string("!"));
			inline PDefH(bool, ExtractModifier, TermNode& term,
				const ValueObject& mod = string("!"))
				ImplRet(ExtractModifier(term.GetContainerRef(), mod))
				//@}

				//! \brief ��Լ���ܴ������η����
				template<typename _func>
			void
				ReduceWithModifier(TermNode& term, ContextNode& ctx, _func f)
			{
				const bool mod(ExtractModifier(term));

				if (IsBranch(term))
					f(term, ctx, mod);
				else
					throw InvalidSyntax("Argument not found.");
			}
			//@}


			/*!
			\brief ���ʽڵ㲢����һԪ������
			*/
			//@{
			template<typename _func, typename... _tParams>
			void
				CallUnary(_func f, TermNode& term, _tParams&&... args)
			{
				QuoteN(term);

				leo::EmplaceFromCall(term.Value,
					leo::make_expanded<void(TermNode&, _tParams&&...)>(std::move(f)),
					Deref(std::next(term.begin())), lforward(args)...);
				term.ClearContainer();
			}

			template<typename _type, typename _func, typename... _tParams>
			void
				CallUnaryAs(_func f, TermNode& term, _tParams&&... args)
			{
				Forms::CallUnary([&](TermNode& node) {
					// XXX: Blocked. 'lforward' cause G++ 5.3 crash: internal compiler
					//	error: Segmentation fault.
					return leo::make_expanded<void(_type&, _tParams&&...)>(std::move(f))(
						leo::Access<_type>(node), std::forward<_tParams&&>(args)...);
				}, term);
			}
			//@}

			/*!
			\brief ����һԪ����չ�����õĺ�������
			\todo ʹ�� C++1y lambda ���ʽ���档
			*/
			//@{
			//! \sa Forms::CallUnary
			template<typename _func>
			struct UnaryExpansion
			{
				_func Function;

				template<typename... _tParams>
				void
					operator()(TermNode& term, _tParams&&... args)
				{
					Forms::CallUnary(Function, term, lforward(args)...);
				}
			};


			//! \sa Forms::CallUnaryAs
			template<typename _type, typename _func>
			struct UnaryAsExpansion
			{
				_func Function;

				template<typename... _tParams>
				void
					operator()(TermNode& term, _tParams&&... args)
				{
					Forms::CallUnaryAs<_type>(Function, term, lforward(args)...);
				}
			};
			//@}

			/*!
			\brief ע��һԪ������
			*/
			template<typename _func>
			void
				RegisterUnaryFunction(ContextNode& node, const string& name, _func f)
			{
				RegisterFunction(node, name, UnaryExpansion<_func>{f}, IsBranch);
			}
			template<typename _type, typename _func>
			void
				RegisterUnaryFunction(ContextNode& node, const string& name, _func f)
			{
				RegisterFunction(node, name, UnaryAsExpansion<_type, _func>{f}, IsBranch);
			}

			//@{
			/*!
			\note �ڽڵ��� bool ����ָ��ʹ�ö�����������ã��ض��壩��
			\note ֧�����η���
			\note ʵ��������ʽ��ֵ�������ʽ��ת�ƣ��ڱ�ʶ���滻ʱ���ܽ�һ����ֵ��
			\sa ReduceWithModifier
			*/
			//@{
			/*!
			\brief ����������
			\throw InvalidSyntax �ڵ����Ƿ������� DefineOrSetFor �׳����쳣��
			\sa DefineOrSetFor
			\sa Lambda

			�޶������������ʹ�� RegisterFormContextHandler ע�������Ĵ�������
			������ʽ�ο��ķ���
			$define|$set [!] <variable> <expression>
			$define|$set [!] (<variable> <formals>) <body>
			*/
			LS_API void
				DefineOrSet(TermNode&, ContextNode&, bool);

			/*!
			\brief ��������ñ�ʶ����ֵΪָ�����
			\note �����ֱ�Ϊ��ʶ��������Լ��������ġ�ʹ�ö����Լ��Ƿ�������η���
			\throw InvalidSyntax ��ʶ������������
			\sa CategorizeLiteral
			\sa DefineValue
			\sa EvaluateIdentifier
			\sa RedefineValue

			��������ò���ָ����ֵ�����ȼ���ʶ��������������
			�ų���ѡ���⣬���ڶ��������б�
			����������Դ��б��һ����Ϊ���ơ�ʣ����Ϊ������ �� ����
			����ֱ�Ӷ���ֵ��
			*/
			LS_API void
				DefineOrSetFor(const string&, TermNode&, ContextNode&, bool, bool);
			//@}

			/*!
			\brief ��������Ϊ�����б���ȡ �� ����Ĳ�����
			\throw InvalidSyntax �����ظ��Ĳ�����
			*/
			LS_API leo::shared_ptr<vector<string>>
				ExtractLambdaParameters(const TermNode::Container&);

			/*!
			\brief �� ���󣺲���һ������ǰ�����ĵĹ��̡�
			\note ʵ��������ʽ�������������ʽ��ת�ƣ��ں���Ӧ��ʱ���ܽ�һ����ֵ��
			\exception InvalidSyntax �쳣�������� ExtractLambdaParameters �׳���
			\sa EvaluateIdentifier
			\sa ExtractLambdaParameters
			\todo �Ż���������

			ʹ�� ExtractLambdaParameters �������б�����Ͱ󶨱�����
			Ȼ�����ýڵ��ֵΪ��ʾ �� ����������Ĵ�������
			��ʹ�� RegisterFormContextHandler ע�������Ĵ�������
			������ʽ�ο��ķ���
			$lambda <formals> <body>
			*/
			LS_API void
				Lambda(TermNode&, ContextNode&);
			//@}

			/*!
			\brief ���� UTF-8 �ַ�����ϵͳ��������� int ���͵Ľ�������ֵ�С�
			\sa usystem
			*/
			LS_API void
				CallSystem(TermNode&);


			//@{
			/*!
			\brief �Ƚ����������ֵ��ȡ�
			\sa leo::HoldSame
			*/
			LS_API void
				EqualReference(TermNode&);

			/*!
			\brief �Ƚ����������ֵ��ȡ�
			\sa leo::ValueObject
			*/
			LS_API void
				EqualValue(TermNode&);
			//@}

		} // namespace Forms;
	}
} // namespace scheme;

#endif