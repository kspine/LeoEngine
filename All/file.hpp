#pragma once
#include "ldef.h"
#include "leoint.hpp"
#include <base.h>
#include <Abstract.hpp>

#include <memory>
#include <string>
#include <functional>
#include <cstddef>

namespace leo
{
	namespace win
	{
		class File : noncopyable,ABSTRACT
		{
		public:
			enum : uint64
			{
				INVALIDE_SIZE = (uint64)-1
			};

			enum : uint32
			{
				//Ȩ�޿���
				TO_READ = 0X00000001,//��
				TO_WRITE = 0X00000002,//д

				//������Ϊ����<ָ��TO_WRITE��>
				NO_CREATE =0X00000004,//�ļ�������ʧ��
				FAIL_IF_EXISTS = 0X00000008,//�ļ�����ʧ��

				//����
				NO_BUFFERING = 0X00000010,
				WRITE_THROUGH = 0X00000020,
				DEL_ON_CLOSE =0X00000040,
				NO_TRUNC = 0X00000080,//TO_WRITE�´��ļ��������������
			};

		public:
			static std::unique_ptr<File> Open(const std::wstring &wsoPath, std::uint32_t u32Flags);

			static std::unique_ptr<File> OpenNoThrow(const std::wstring &wsoPath, std::uint32_t u32Flags);

		public:
			std::uint64_t GetSize() const;
			void Resize(std::uint64_t u64NewSize);
			void Clear();

			std::size_t Read(void *pBuffer, std::size_t uBytesToRead, std::uint64_t u64Offset) const;
			void Write(std::uint64_t u64Offset, const void *pBuffer, std::size_t uBytesToWrite);

			// 1. fnAsyncProc ���ǻᱻִ��һ�Σ���ʹ��ȡ��д�����ʧ�ܣ�
			// 2. ��� IO ������һ����ɣ����糢���� 64 λ������һ�ζ�ȡ���� 4GiB �����ݣ���������Ϊ��ν��С�
			//    ��������������£�ֻ�е�һ�εĲ������첽�Ĳ��һᴥ���ص���
			// 3. ���еĻص������������׳��쳣������������£��쳣���ڶ�ȡ��д�������ɻ�ʧ�ܺ������׳���
			// 4. ���ҽ��� fnAsyncProc �ɹ��������첽�����ɹ��� fnCompleteCallback �Żᱻִ�С�
			std::size_t Read(
				void *pBuffer, std::size_t uBytesToRead, std::uint64_t u64Offset,
				const std::function<void()> &fnAsyncProc,
				const std::function<void()> &fnCompleteCallback
				) const;
			void Write(
				std::uint64_t u64Offset, const void *pBuffer, std::size_t uBytesToWrite,
				const std::function<void()> &fnAsyncProc,
				const std::function<void()> &fnCompleteCallback
				);

			void Flush() const;
		};
	}
	namespace win
	{

		namespace file
		{
			enum class FILE_TYPE :std::uint8_t
			{
				DDS = 0,
				TGA = 1,
				OTHER_TEX_BEGIN = 2,
				OTHER_TEX_END = 10
			};
			bool FileExist(const wchar_t * filename);
			std::wstring GetDirectoryFromFileName(const wchar_t * filename);
			std::wstring GetFileNameWithoutExt(const wchar_t * filename);
			FILE_TYPE GetFileExt(const wchar_t* filename);

			bool FileExist(const std::wstring& filename);
			std::wstring GetDirectoryFromFileName(const std::wstring& filename);
			std::wstring GetFileNameWithoutExt(const std::wstring& filename);
			FILE_TYPE GetFileExt(const std::wstring& filename);
			const size_t max_path = 260;

			class TempFile
			{
			public:
				TempFile();
				TempFile(const std::wstring & path, const std::wstring & prefix);
				TempFile(const wchar_t * path, const wchar_t * prefix);
				~TempFile();
				const wchar_t* c_str() const;

				TempFile(const TempFile&) = delete;
				void operator=(const TempFile&) = delete;
				TempFile(TempFile&& rvalue) = delete;
			private:
				wchar_t mTempFileName[max_path];
			public:
				static void Path(wchar_t *path);
				static void Prefix(wchar_t *prefix);
			private:
				static const size_t max_prefix = 8;
				static wchar_t mPath[max_path];
				static wchar_t mPrefix[max_prefix];
			};
			void HuffManEncode(wchar_t * src, wchar_t * dst);
			void HuffManDecode(wchar_t * src, wchar_t * dst);
		}
	}
}