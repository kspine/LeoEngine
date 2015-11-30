#include <BaseMacro.h>
#include <limits>


namespace  leo
{

	namespace HUD
	{

		struct Size;
		struct Rect;


		/*!
		\brief ��Ļ��Ԫ�顣
		\warning ����������
		*/
		template<typename _type>
		class GBinaryGroup
		{
		public:
			static const GBinaryGroup Invalid; //!< ��Ч��������Ļ����ϵ�У�����

			_type X = 0, Y = 0; //!< ������

								/*!
								\brief �޲������졣
								\note ���ʼ����
								\since build 319
								*/
			lconstfn DefDeCtor(GBinaryGroup)
				/*!
				\brief ���ƹ��죺Ĭ��ʵ�֡�
				*/
				lconstfn DefDeCopyCtor(GBinaryGroup)
				/*!
				\brief ���죺ʹ�� Size ����
				\since build 319
				*/
				explicit lconstfn
				GBinaryGroup(const Size&) lnothrow;
			/*!
			\brief ���죺ʹ�� Rect ����
			\since build 319
			*/
			explicit lconstfn
				GBinaryGroup(const Rect&) lnothrow;
			/*!
			\brief ���죺ʹ������������
			\tparam _tScalar1 ��һ�����������͡�
			\tparam _tScalar2 �ڶ������������͡�
			\warning ģ������� _type ���Ų�ͬʱ��ʽת�����ܸı���ţ�����֤Ψһ�����
			*/
			template<typename _tScalar1, typename _tScalar2>
			lconstfn
				GBinaryGroup(_tScalar1 x, _tScalar2 y) lnothrow
				: X(_type(x)), Y(_type(y))
			{}
			/*!
			\brief ���죺ʹ�ô����ԡ�
			\note ʹ�� std::get ȡ��������ȡǰ����������
			*/
			template<typename _tPair>
			lconstfn
				GBinaryGroup(const _tPair& pr) lnothrow
				: X(std::get<0>(pr)), Y(std::get<1>(pr))
			{}

			//! \since build 554
			DefDeCopyAssignment(GBinaryGroup)

				/*!
				\brief �����㣺ȡ�ӷ���Ԫ��
				*/
				lconstfn PDefHOp(GBinaryGroup, -, ) const lnothrow
				ImplRet(GBinaryGroup(-X, -Y))

				/*!
				\brief �ӷ���ֵ��
				*/
				PDefHOp(GBinaryGroup&, +=, const GBinaryGroup& val) lnothrow
				ImplRet(yunseq(X += val.X, Y += val.Y), *this)
				/*!
				\brief ������ֵ��
				*/
				PDefHOp(GBinaryGroup&, -=, const GBinaryGroup& val) lnothrow
				ImplRet(yunseq(X -= val.X, Y -= val.Y), *this)

				lconstfn DefGetter(const lnothrow, _type, X, X)
				lconstfn DefGetter(const lnothrow, _type, Y, Y)

				DefSetter(_type, X, X)
				DefSetter(_type, Y, Y)

				/*!
				\brief �ж��Ƿ�����Ԫ�ء�
				*/
				lconstfn DefPred(const lnothrow, Zero, X == 0 && Y == 0)

				/*!
				\brief ѡ��������á�
				\note �ڶ�����Ϊ true ʱѡ���һ����������ѡ��ڶ�������
				*/
				//@{
				PDefH(_type&, GetRef, bool b = true) lnothrow
				ImplRet(b ? X : Y)
				PDefH(const _type&, GetRef, bool b = true) const lnothrow
				ImplRet(b ? X : Y)
				//@}
		};

		//! \relates GBinaryGroup
		//@{
		template<typename _type>
		const GBinaryGroup<_type> GBinaryGroup<_type>::Invalid{
			std::numeric_limits<_type>::lowest(), std::numeric_limits<_type>::lowest() };

		template<typename _type>
		lconstfn bool
			operator==(const GBinaryGroup<_type>& x, const GBinaryGroup<_type>& y) lnothrow
		{
			return x.X == y.X && x.Y == y.Y;
		}

		template<typename _type>
		lconstfn bool
			operator!=(const GBinaryGroup<_type>& x, const GBinaryGroup<_type>& y) lnothrow
		{
			return !(x == y);
		}

		template<typename _type>
		lconstfn GBinaryGroup<_type>
			operator+(const GBinaryGroup<_type>& x, const GBinaryGroup<_type>& y) lnothrow
		{
			return GBinaryGroup<_type>(x.X + y.X, x.Y + y.Y);
		}

		template<typename _type>
		lconstfn GBinaryGroup<_type>
			operator-(const GBinaryGroup<_type>& x, const GBinaryGroup<_type>& y) lnothrow
		{
			return GBinaryGroup<_type>(x.X - y.X, x.Y - y.Y);
		}

		template<typename _type, typename _tScalar>
		lconstfn GBinaryGroup<_type>
			operator*(const GBinaryGroup<_type>& val, _tScalar l) lnothrow
		{
			return GBinaryGroup<_type>(val.X * l, val.Y * l);
		}

		template<class _tBinary>
		lconstfn _tBinary
			Transpose(const _tBinary& obj) lnothrow
		{
			return _tBinary(obj.Y, obj.X);
		}

		template<size_t _vIdx, typename _type>
		lconstfn _type&
			get(GBinaryGroup<_type>& val)
		{
			static_assert(_vIdx < 2, "Invalid index found.");

			return _vIdx == 0 ? val.X : val.Y;
		}
		template<size_t _vIdx, typename _type>
		lconstfn const _type&
			get(const GBinaryGroup<_type>& val)
		{
			static_assert(_vIdx < 2, "Invalid index found.");

			return _vIdx == 0 ? val.X : val.Y;
		}
	}
}
