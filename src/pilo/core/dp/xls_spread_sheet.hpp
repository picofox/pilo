#ifndef _pilo_core_dp_xls_spread_sheet_hpp_
#define _pilo_core_dp_xls_spread_sheet_hpp_

#include "../../external/OpenXLSX/OpenXLSX.hpp"
#include "../io/path.hpp"
#include "../io/io.hpp"
#include "../../tlv.hpp"

namespace pilo
{
	namespace core
	{
		namespace dp
		{
			class xls_spread_sheet
			{
			public:
				const static ::pilo::u32_t invalid_count = 0xFFFFFFFF;

			public:
				xls_spread_sheet();
				~xls_spread_sheet();

				::pilo::err_t open(const char* path_str, ::pilo::core::io::creation_mode cm, ::pilo::predefined_pilo_path prefix);

				void close();
				void save();
				::pilo::u32_t sheet_count() const;
				::pilo::i32_t worksheet_count()const;
				::pilo::u32_t row_count(const std::string& sheet_name) const;
				::pilo::u32_t col_count(const std::string& sheet_name) const;

				::pilo::tlv* value(const std::string& name, ::pilo::u32_t row, ::pilo::u32_t col) const;
				::pilo::tlv* value(::pilo::u16_t idx, ::pilo::u32_t row, ::pilo::u32_t col) const;

				template<typename T>
				::pilo::err_t value(T& value_ret, ::pilo::u16_t idx, ::pilo::u32_t row, ::pilo::u32_t col) const
				{
					try
					{
						auto wks = _doc.workbook().worksheet(idx);
						OpenXLSX::XLCellAssignable cell = wks.cell(row, (::pilo::u16_t)col);						
						value_ret = cell.value().get<T>();
						return PILO_OK;
					}
					catch (const std::exception&)
					{
						return ::pilo::mk_perr(PERR_NON_EXIST);
					}
				}

				template<typename T>
				::pilo::err_t value(T& value_ret, const std::string& name, ::pilo::u32_t row, ::pilo::u32_t col) const
				{
					try
					{
						auto wks = _doc.workbook().worksheet(name);
						OpenXLSX::XLCellAssignable cell = wks.cell(row, (::pilo::u16_t)col);
						value_ret = cell.value().get<T>();
						return PILO_OK;
					}
					catch (const std::exception&)
					{
						return ::pilo::mk_perr(PERR_NON_EXIST);
					}
				}

				::pilo::u32_t row_count() const;
				::pilo::u32_t col_count() const;
			
				::pilo::tlv* row_values(const std::string& name, ::pilo::u32_t row, ::pilo::u32_t col_beg, ::std::initializer_list<::pilo::i16_t> types);
				::pilo::tlv* row_values(::pilo::u16_t idx, ::pilo::u32_t row, ::pilo::u32_t col_beg, ::std::initializer_list<::pilo::i16_t> types);

				::pilo::tlv* row_values_selected(::pilo::u16_t idx, ::pilo::u32_t row, ::std::initializer_list<::pilo::u32_t> indices, ::std::initializer_list<::pilo::i16_t> types);
			
				::pilo::err_t select_work_sheet(const std::string& sheet_name);
				::pilo::err_t select_work_sheet(::pilo::u16_t idx);
				::pilo::tlv* row_values(::pilo::u32_t row, ::pilo::u32_t col_beg, ::std::initializer_list<::pilo::i16_t> types);

				template<typename T>
				::pilo::err_t value(T& value_ret, ::pilo::u32_t row, ::pilo::u32_t col) const
				{
					try
					{
						OpenXLSX::XLCellAssignable cell = _work_sheet.cell(row, (::pilo::u16_t)col);
						value_ret = cell.value().get<T>();
						return PILO_OK;
					}
					catch (const std::exception&)
					{
						return ::pilo::mk_perr(PERR_NON_EXIST);
					}
				}
				::pilo::err_t value(::pilo::tlv* tlv_ptr, ::pilo::u32_t row, ::pilo::u32_t col) const;
				::pilo::tlv* value(::pilo::u32_t row, ::pilo::u32_t col) const;
				::pilo::err_t strvalue(std::string& str, ::pilo::u32_t row, ::pilo::u32_t col) const;
				

			protected:
				::pilo::tlv* _value(const OpenXLSX::XLCellAssignable& cell) const;
				::pilo::tlv* _value(const OpenXLSX::XLCellAssignable& cell, ::pilo::i16_t t) const;
				::pilo::err_t _value(::pilo::tlv* tlv_ptr, const OpenXLSX::XLCellAssignable& cell) const;
				::pilo::err_t _value(std::string& str, const OpenXLSX::XLCellAssignable& cell) const;

			protected:
				::pilo::core::io::path	_m_path;
				OpenXLSX::XLDocument	_doc;
				bool					_open_for_write;
				OpenXLSX::XLWorksheet	_work_sheet;
			};

		}
	}
}


#endif