#ifndef LScheme_LSchemeREPL_H
#define LScheme_LSchemeREPL_H 1

#include "LScheme.h"

namespace scheme {
	namespace v1 {
		/*
		\brief REPL �����ġ�
		\warning ����������

		REPL ��ʾ��ȡ-��ֵ-���ѭ����
		ÿ��ѭ������һ�η��롣
		��ֻ��һ�������Ŀ���չʵ�֡�����ͨ���������ݳ�Ա���ơ�
		*/
		class LS_API REPLContext
		{
		public:
			//! \brief �����ĸ��ڵ㡣
			ContextNode Root{};
			//! \brief Ԥ����ڵ㣺ÿ�η���ʱԤ�ȴ�����õĹ������̡�
			TermPasses Preprocess{};
			//! \brief ��������̣�ÿ�η����й�Լ�ص�������õĹ������̡�
			EvaluationPasses ListTermPreprocess{};

			/*!
			\brief ���죺ʹ��Ĭ�ϵĽ��͡�
			\note ����ָ���Ƿ����öԹ�Լ��Ƚ��и��١�
			\sa ListTermPreprocess
			\sa SetupDefaultInterpretation
			\sa SetupTraceDepth
			*/
			REPLContext(bool = {});

			/*!
			\brief �����������벢Ԥ�������й�Լ��
			\sa SContext::Analyze
			\sa Preprocess
			\sa Reduce
			*/
			//@{
			void
				Process(TermNode&);
			TermNode
				Process(const TokenList&);
			TermNode
				Process(const Session&);
			//@}

			/*!
			\brief ִ��ѭ�����Էǿ�������з��롣
			\pre ���ԣ��ַ���������ָ��ǿա�
			\throw LoggedEvent ����Ϊ�մ���
			\sa Process
			*/
			TermNode
				Perform(string_view);
		};

		namespace Forms
		{
			/*!
			\brief ���� REPL ���Է��뵥Ԫ��Լ����ֵ��
			*/
			LS_API void
				Eval(TermNode&, const REPLContext&);
		}
	}
}

#endif
