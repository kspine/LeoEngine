/*!	\defgroup preprocessor_helpers Perprocessor Helpers
\brief Ԥ������ͨ�����ֺꡣ
\since build 1.02
*/
//@{

//\brief �滻�б�
//\note ͨ�����ű����������ڴ��ݴ����ŵĲ�����
#define LPP_Args(...) __VA_ARGS__

//! \brief �滻Ϊ�յ�Ԥ����Ǻš�
#define LPP_Empty

/*!
\brief �滻Ϊ���ŵ�Ԥ����Ǻš�
\note �����ڴ�����ʵ�ʲ����г��ֵĶ��š�
*/
#define LPP_Comma ,

/*
\brief �Ǻ����ӡ�
\sa LPP_Join
*/
#define LPP_Concat(x,y) x ## y

/*
\brief �����滻�ļǺ����ӡ�
\see ISO WG21/N4140 16.3.3[cpp.concat]/3 ��
\see http://gcc.gnu.org/onlinedocs/cpp/Concatenation.html ��
\see https://www.securecoding.cert.org/confluence/display/cplusplus/PRE05-CPP.+Understand+macro+replacement+when+concatenating+tokens+or+performing+stringification ��

ע�� ISO C++ δȷ���궨���� # �� ## ��������ֵ˳��
ע��궨���� ## ���������ε���ʽ����Ϊ��ʱ���˺겻�ᱻչ����
*/
#define LPP_Join(x,y) LPP_Concat(x,y)
//@}


/*!
\brief ʵ�ֱ�ǩ��
\since build 1.02
\todo �������ʵ�ֵı�Ҫ֧�֣��ɱ�����ꡣ
*/
#define limpl(...) __VA_ARGS__

#define lnothrow noexcept
