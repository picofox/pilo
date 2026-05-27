#include	"./xls_spread_sheet.hpp"
#include	"../process/context.hpp"

pilo::core::dp::xls_spread_sheet::xls_spread_sheet(OpenXLSX::XLWorksheet ws) : _work_sheet(ws)
{

}

pilo::core::dp::xls_spread_sheet::~xls_spread_sheet()
{

}


::pilo::u32_t pilo::core::dp::xls_spread_sheet::row_count() const
{
	try 
	{
		return _work_sheet.rowCount();
	}
	catch (const std::exception&)
	{
		return xls_spread_sheet::invalid_count;
	}
}

::pilo::u32_t pilo::core::dp::xls_spread_sheet::col_count() const
{
	try
	{
		return _work_sheet.columnCount();
	}
	catch (const std::exception&)
	{
		return xls_spread_sheet::invalid_count;
	}
}


::pilo::tlv* pilo::core::dp::xls_spread_sheet::row_values(::pilo::u32_t row, ::pilo::u32_t col_beg, ::std::initializer_list<::pilo::i16_t> types)
{
	::pilo::tlv* tlvp = PILO_CONTEXT->allocate_tlv();
	tlvp->set_array_type(::pilo::core::rtti::wired_type::value_type_tlv);

	::pilo::u32_t i = 0;
	for (::pilo::i16_t t : types) {
		OpenXLSX::XLCellAssignable cell = _work_sheet.cell(row, (::pilo::u16_t)(col_beg + i));
		::pilo::tlv* p = _value(cell, t);
		if (p == nullptr) {
			PILO_CONTEXT->deallocate_tlv(tlvp);
			return nullptr;
		}
		tlvp->push_back(p, -1, true);
		i++;
	}

	return tlvp;
}

::pilo::tlv* pilo::core::dp::xls_spread_sheet::_value(const OpenXLSX::XLCellAssignable& cell, ::pilo::i16_t t) const
{
	try
	{
		::pilo::tlv* tlvp = PILO_CONTEXT->allocate_tlv();
		if (cell.empty())
			return tlvp;

		if (t == ::pilo::core::rtti::wired_type::value_type_i8) {
			::pilo::i8_t iv = cell.value().get<::pilo::i8_t>();
			tlvp->set(iv);
		} 
		else if (t == ::pilo::core::rtti::wired_type::value_type_u8) {
			::pilo::u8_t iv = cell.value().get<::pilo::u8_t>();
			tlvp->set(iv);		}
		else if (t == ::pilo::core::rtti::wired_type::value_type_i16) {
			::pilo::i16_t iv = cell.value().get<::pilo::i16_t>();
			tlvp->set(iv);
		}
		else if (t == ::pilo::core::rtti::wired_type::value_type_u16) {
			::pilo::u16_t iv = cell.value().get<::pilo::u16_t>();
			tlvp->set(iv);
		}
		else if (t == ::pilo::core::rtti::wired_type::value_type_i32) {
			::pilo::i32_t iv = cell.value().get<::pilo::i32_t>();
			tlvp->set(iv);
		}
		else if (t == ::pilo::core::rtti::wired_type::value_type_u32) {
			::pilo::u32_t iv = cell.value().get<::pilo::u32_t>();
			tlvp->set(iv);
		}
		else if (t == ::pilo::core::rtti::wired_type::value_type_i64) {
			::pilo::i64_t iv = cell.value().get<::pilo::i64_t>();
			tlvp->set(iv);
		}
		else if (t == ::pilo::core::rtti::wired_type::value_type_u64) {
			::pilo::u64_t iv = cell.value().get<::pilo::u64_t>();
			tlvp->set(iv);
		}
		else if (t == ::pilo::core::rtti::wired_type::value_type_f32) {
			::pilo::f32_t fv = cell.value().get<::pilo::f32_t>();
			tlvp->set(fv);
		}
		else if (t == ::pilo::core::rtti::wired_type::value_type_f64) {
			::pilo::f64_t fv = cell.value().get<::pilo::f64_t>();
			tlvp->set(fv);
		}
		else if (t == ::pilo::core::rtti::wired_type::value_type_boolean) {
			bool bv = cell.value().get<bool>();
			tlvp->set(bv);
		}
		else if (t == ::pilo::core::rtti::wired_type::value_type_str) {
			std::string str = cell.value().get<std::string>();
			tlvp->set(str);
		}
		else {
			PILO_CONTEXT->deallocate_tlv(tlvp);
			return nullptr;
		}

		return tlvp;
	}
	catch (const std::exception&)
	{
		return nullptr;
	}

}

::pilo::err_t pilo::core::dp::xls_spread_sheet::_value(::pilo::tlv* tlv_ptr, const OpenXLSX::XLCellAssignable& cell) const
{
	try
	{

		if (cell.value().type() == OpenXLSX::XLValueType::Empty) {
			tlv_ptr->clear();
			return PILO_OK;
		}

		OpenXLSX::XLValueType ctp = cell.value().type();
		if (ctp == OpenXLSX::XLValueType::Integer) {
			::pilo::i64_t iv = cell.value().get<::pilo::i64_t>();
			tlv_ptr->set(iv);

		}
		else if (ctp == OpenXLSX::XLValueType::Float) {
			::pilo::f64_t fv = cell.value().get<::pilo::f64_t>();
			tlv_ptr->set(fv);

		}
		else if (ctp == OpenXLSX::XLValueType::String) {
			std::string str = cell.value().get<std::string>();
			tlv_ptr->set(str);
		}
		else if (ctp == OpenXLSX::XLValueType::Boolean) {
			bool bv = cell.value().get<bool>();
			tlv_ptr->set(bv);
		}
		else if (ctp == OpenXLSX::XLValueType::Error) {
			return ::pilo::mk_perr(PERR_INC_DATA);
		}
		else if (ctp == OpenXLSX::XLValueType::Empty) {
			tlv_ptr->reset();
		}
		return PILO_OK;
	}
	catch (const std::exception&)
	{
		return ::pilo::mk_perr(PERR_INC_DATA);
	}
}

::pilo::err_t pilo::core::dp::xls_spread_sheet::_value(std::string& str, const OpenXLSX::XLCellAssignable& cell) const
{
	try
	{
		if (cell.value().type() == OpenXLSX::XLValueType::Empty) {
			str.clear();
			return PILO_OK;
		}		

		char buffer[128] = {0};

		OpenXLSX::XLValueType ctp = cell.value().type();
		if (ctp == OpenXLSX::XLValueType::Integer) {
			::pilo::i64_t iv = cell.value().get<::pilo::i64_t>();
			::pilo::core::string::number_to_string(buffer, sizeof(buffer), iv, nullptr);
			str.assign(buffer);
		}
		else if (ctp == OpenXLSX::XLValueType::Float) {
			::pilo::f64_t fv = cell.value().get<::pilo::f64_t>();
			::pilo::core::string::number_to_string(buffer, sizeof(buffer), fv, nullptr);
			str.assign(buffer);
		}
		else if (ctp == OpenXLSX::XLValueType::String) {
			str = cell.value().get<std::string>();
		}
		else if (ctp == OpenXLSX::XLValueType::Boolean) {
			bool bv = cell.value().get<bool>();
			if (bv) {
				str = "true";
			}
			else {
				str = "false";
			}
		}
		else if (ctp == OpenXLSX::XLValueType::Error) {
			return ::pilo::mk_perr(PERR_INC_DATA);
		}
		return PILO_OK;
	}
	catch (const std::exception&)
	{
		return ::pilo::mk_perr(PERR_INC_DATA);
	}
}

::pilo::err_t pilo::core::dp::xls_spread_sheet::value(::pilo::tlv* tlv_ptr, ::pilo::u32_t row, ::pilo::u32_t col) const
{
	try
	{
		OpenXLSX::XLCellAssignable cell = _work_sheet.cell(row, (::pilo::u16_t)col);
		return this->_value(tlv_ptr, cell);
	}
	catch (const std::exception&)
	{
		return ::pilo::mk_perr(PERR_INC_DATA);
	}
}

::pilo::tlv* pilo::core::dp::xls_spread_sheet::value(::pilo::u32_t row, ::pilo::u32_t col) const
{
	try
	{
		OpenXLSX::XLCellAssignable cell = _work_sheet.cell(row, (::pilo::u16_t)col);
		return this->_value(cell);
	}
	catch (const std::exception&)
	{
		return nullptr;
	}
}

::pilo::err_t pilo::core::dp::xls_spread_sheet::strvalue(std::string& str, ::pilo::u32_t row, ::pilo::u32_t col) const
{
	try
	{
		OpenXLSX::XLCellAssignable cell = _work_sheet.cell(row, (::pilo::u16_t)col);
		return this->_value(str, cell);
	}
	catch (const std::exception&)
	{
		return ::pilo::mk_perr(PERR_INC_DATA);
	}
}


::pilo::tlv* pilo::core::dp::xls_spread_sheet::_value(const OpenXLSX::XLCellAssignable& cell) const
{
	try
	{
		::pilo::tlv* tlvp = nullptr;
		if (cell.empty())
			return PILO_CONTEXT->allocate_tlv();

		OpenXLSX::XLValueType ctp = cell.value().type();
		if (ctp == OpenXLSX::XLValueType::Integer) {
			::pilo::i64_t iv = cell.value().get<::pilo::i64_t>();
			tlvp = PILO_CONTEXT->allocate_tlv();
			tlvp->set(iv);
			
		} else if (ctp == OpenXLSX::XLValueType::Float) {
			::pilo::f64_t fv = cell.value().get<::pilo::f64_t>();
			tlvp = PILO_CONTEXT->allocate_tlv();
			tlvp->set(fv);

		} else if (ctp == OpenXLSX::XLValueType::String) {
			std::string str = cell.value().get<std::string>();
			tlvp = PILO_CONTEXT->allocate_tlv();
			tlvp->set(str);
		}
		else if (ctp == OpenXLSX::XLValueType::Boolean) {
			bool bv = cell.value().get<bool>();
			tlvp = PILO_CONTEXT->allocate_tlv();
			tlvp->set(bv);
		}
		else if (ctp == OpenXLSX::XLValueType::Error) {
			return nullptr;
		}
		else if (ctp == OpenXLSX::XLValueType::Empty) {
			return PILO_CONTEXT->allocate_tlv();
		}

		return tlvp;
	}
	catch (const std::exception&)
	{
		return nullptr;
	}

}

pilo::core::dp::xls_spread_document::xls_spread_document() :_open_for_write(false)
{
	_doc.suppressWarnings();
}

pilo::core::dp::xls_spread_document::~xls_spread_document()
{
	close();
}

::pilo::err_t pilo::core::dp::xls_spread_document::open(const char* path_str, ::pilo::core::io::creation_mode cm, ::pilo::predefined_pilo_path prefix)
{
	if (_doc.isOpen())
		return ::pilo::mk_perr(PERR_EXIST);

	this->_m_path.set(path_str, prefix);
	if (this->_m_path.invalid()) {
		return ::pilo::mk_perr(PERR_INVALID_PATH);
	}

	::pilo::err_t err = this->_m_path.ensure_parent_path_exist();
	if (err != PILO_OK) {
		return ::pilo::mk_perr(PERR_INVALID_PATH);
	}

	bool is_exist = false;
	::pilo::i8_t fsnt = this->_m_path.get_fs_info(nullptr, nullptr);
	if (fsnt == ::pilo::core::io::path::path_type_na) {
		is_exist = false;
	}
	else if (fsnt == ::pilo::core::io::path::fs_node_type_file) {
		is_exist = true;
	}
	else {
		return ::pilo::mk_perr(PERR_NOT_A_REG_FILE);
	}

	if (cm == ::pilo::core::io::creation_mode::create_always) {
		try
		{
			_doc.create(std::string(_m_path.fullpath()), true);
			_open_for_write = true;
		}
		catch (const std::exception&)
		{
			return ::pilo::mk_perr(PERR_IO_CREATE_FAIL);
		}
	}
	else if (cm == ::pilo::core::io::creation_mode::create_neo) {
		try
		{
			if (is_exist)
				return ::pilo::mk_perr(PERR_EXIST);
			_doc.create(std::string(_m_path.fullpath()), false);
			_open_for_write = true;
		}
		catch (const std::exception&)
		{
			return ::pilo::mk_perr(PERR_IO_CREATE_FAIL);
		}
	}
	else if (cm == ::pilo::core::io::creation_mode::open_existing) {
		try
		{
			if (!is_exist)
				return ::pilo::mk_perr(PERR_NON_EXIST);

			_doc.open(std::string(_m_path.fullpath()));
			_open_for_write = false;
		}
		catch (const std::exception&)
		{
			return ::pilo::mk_perr(PERR_IO_CREATE_FAIL);
		}
	}
	else if (cm == ::pilo::core::io::creation_mode::open_always) {
		try
		{
			_doc.open(std::string(_m_path.fullpath()));
			_open_for_write = false;
		}
		catch (const std::exception&)
		{
			try
			{
				_doc.create(std::string(_m_path.fullpath()), true);
				_open_for_write = true;
			}
			catch (const std::exception&)
			{
				return ::pilo::mk_perr(PERR_IO_CREATE_FAIL);
			}
		}
	}
	else {
		return ::pilo::mk_perr(PERR_INVALID_PARAM);
	}

	_doc.suppressWarnings();

	return PILO_OK;
}

void pilo::core::dp::xls_spread_document::close()
{
	if (_doc.isOpen()) {
		if (_open_for_write) {
			_doc.save();
		}
		_doc.close();
	}
}

void pilo::core::dp::xls_spread_document::save()
{
	_doc.save();
}

void pilo::core::dp::xls_spread_document::save_as(const std::string& filepathstr, bool force_overwrite)
{
	_doc.saveAs(filepathstr, force_overwrite);
}

::pilo::u32_t pilo::core::dp::xls_spread_document::sheet_count() const
{
	return _doc.workbook().sheetCount();
}

::pilo::u32_t pilo::core::dp::xls_spread_document::worksheet_count() const
{
	return _doc.workbook().worksheetCount();
}

::pilo::err_t pilo::core::dp::xls_spread_document::get_all_worksheet_names(std::vector<std::string>& names)
{
	try
	{
		::pilo::u32_t cnt = _doc.workbook().worksheetCount();
		for (::pilo::u32_t u = 1; u <= cnt; u++) {
			names.push_back(_doc.workbook().worksheet((::pilo::u16_t)u).name());
		}
	}
	catch (const std::exception&)
	{
		return ::pilo::mk_perr(PERR_UNDEF);
	}
	return PILO_OK;
}

::pilo::core::dp::xls_spread_sheet pilo::core::dp::xls_spread_document::worksheet_by_index(::pilo::u32_t index)
{
	OpenXLSX::XLWorksheet ws = _doc.workbook().worksheet((::pilo::u16_t) index);
	::pilo::core::dp::xls_spread_sheet xws(ws);
	return xws;
}

