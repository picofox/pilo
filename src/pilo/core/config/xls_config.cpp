#include	"xls_config.hpp"
#include	"../dp/xls_spread_sheet.hpp"
#include	<sstream>
#include	"../process/context.hpp"
#include    "../ml/json_tlv_driver.hpp"
#include	"../io/path.hpp"

::pilo::err_t pilo::core::config::xls_config::s_ui_parser(const char* src, const char* ptr, ::pilo::i64_t len, void* ctx)
{
	PMC_UNUSED(src);
	if (ptr == nullptr || *ptr == 0 || len <= 0)
		return ::pilo::mk_perr(PERR_INVALID_PARAM);
	pilo::core::config::xls_config* cfg = (pilo::core::config::xls_config*) ctx;
	std::vector<std::string> vec;
	::pilo::err_t err = PILO_OK;
	::pilo::core::string::iteratable_split(ptr, len, ",", (::pilo::i64_t)1, pilo::core::config::xls_config::s_ui_sub_parser, (void*)&vec, false, true, true, true, &err);
	if (err != PILO_OK)
		return err;
	cfg->_union_indices.push_back(std::move(vec));
	return err;
}


::pilo::err_t pilo::core::config::xls_config::s_ui_sub_parser(const char* , const char* ptr, ::pilo::i64_t len, void* ctx)
{
	if (ptr == nullptr || *ptr == 0 || len <= 0)
		return ::pilo::mk_perr(PERR_INVALID_PARAM);
	std::vector<std::string>* vecp = (std::vector<std::string>*) ctx;
	std::string str(ptr, len);
	vecp->push_back(std::move(str));
	return PILO_OK;
}

pilo::core::config::xls_config::xls_config()
{
	_data = PILO_CONTEXT->allocate_tlv();
	_data->set_array_type(::pilo::core::rtti::wired_type::value_type_tlv);
}

pilo::core::config::xls_config::~xls_config()
{
	reset();
	if (_data != nullptr) {
		PILO_CONTEXT->deallocate_tlv(_data);
		_data = nullptr;
	}
}

::pilo::err_t pilo::core::config::xls_config::parse_union_index(const char* ptr, ::pilo::i64_t len)
{
	::pilo::err_t err = PILO_OK;
	::pilo::core::string::iteratable_split(ptr, len, ";", (::pilo::i64_t) 1, pilo::core::config::xls_config::s_ui_parser, (void*)this, false, true, true, true, &err);
	return err;
}

void pilo::core::config::xls_config::reset()
{
	_cls_name.clear();
	_config_file_name.clear();
	_ns.clear();
	_union_indices.clear();
	_union_indices.shrink_to_fit();
	if (_data != nullptr)
		_data->clear();
	
}

::pilo::u32_t pilo::core::config::xls_config::field_count() const
{
	return (::pilo::u32_t) _fields.size();
}

std::string pilo::core::config::xls_config::to_string() const
{
	std::stringstream ss;
	ss << '\t' << _ns << '.' << _cls_name << ' ' << '(' << _config_file_name << ')' << std::endl;
	ss << '\t' << "UI:" << std::endl;
	for (size_t i = 0; i < _union_indices.size(); i++) {
		ss << "\t\t";
		for (size_t j = 0; j < _union_indices[i].size(); j++) {
			ss << _union_indices[i][j] << ',';
		}
		ss << std::endl;
	}
	ss << '\t' << "Fields:" << std::endl;
	for (size_t i = 0; i < _fields.size(); i++) {
		ss << "\t\t" << _fields[i].to_string() << std::endl;
	}
	ss << std::endl;
	return ss.str();
}

::pilo::err_t pilo::core::config::xls_config::save_config_file(const char* path_str, predefined_pilo_path prefix)
{
	::pilo::core::io::path pth(path_str, prefix);
	pth.append(_config_file_name.c_str());
	if (pth.invalid()) {
		return ::pilo::mk_perr(PERR_INVALID_PATH);
	}
	::pilo::core::ml::json_tlv_driver jdrv(_data);
	return jdrv.save(&pth);
}

void pilo::core::config::xls_config_set::reset()
{
	for (auto i = 0; i < 2; i++) {
		_configs[i].reset();
	}
	_sheet_name.clear();
	_name.clear();
	_desc.clear();
}

::pilo::err_t pilo::core::config::xls_config_set::parse(const std::string& sheet_name, const char* path_str, ::pilo::predefined_pilo_path prefix, std::string& errmsg)
{
	
	PMC_UNUSED(sheet_name);
	PMC_UNUSED(path_str);
	PMC_UNUSED(prefix);
	PMC_UNUSED(errmsg);

	this->reset();

	

	/*

	::pilo::cstr_ref<char> parts[6];
	::pilo::i64_t parts_cnt = 0;
	::pilo::core::dp::xls_spread_sheet xss;
	::pilo::err_t err = xss.open(path_str, ::pilo::core::io::creation_mode::open_existing, prefix);
	if (err != PILO_OK)
		return err;
	::pilo::u32_t rcnt = xss.row_count(sheet_name);
	if (rcnt < 2 || rcnt == ::pilo::core::dp::xls_spread_sheet::invalid_count) {
		return ::pilo::mk_perr(PERR_NON_EXIST);
	}
	err = xss.select_work_sheet(sheet_name);
	if (err != PILO_OK)
		return err;

	_sheet_name = sheet_name;
	std::string var_name;
	std::string tmp_cell_str;
	bool content_start = false;
	for (::pilo::u32_t i = 1; i <= rcnt; i++) {
		err = xss.strvalue(tmp_cell_str, i, 1);
		if (err != PILO_OK)
			return err;
		::pilo::core::string::trim_string(tmp_cell_str);
		if (tmp_cell_str.empty() && !content_start) {
			_compose_errmsg(errmsg, i, 1, "format error! found empty 1st cell");
			return ::pilo::mk_perr(PERR_INC_DATA);
		}

		if (tmp_cell_str[0] == '/') {
			if (tmp_cell_str == "///end") {
				if (!content_start) {
					_compose_errmsg(errmsg, i, 1, "format error! document is incomplete.");
					return ::pilo::mk_perr(PERR_INC_DATA);
				}
				break;
			}
			continue;
		} 
		else if (tmp_cell_str[0] == '$') {
			if (content_start) {
				_compose_errmsg(errmsg, i, 1, "Unacceptable Variable! content parsing has already started: (%s)", tmp_cell_str.c_str());
				return ::pilo::mk_perr(PERR_INC_DATA);
			}
			parts_cnt = ::pilo::core::string::split_fixed(tmp_cell_str.c_str(), (::pilo::i64_t) tmp_cell_str.size(),
				":", 1, parts, 2, false, true, false, false);
			if (parts_cnt != 2) {
				_compose_errmsg(errmsg, i, 1, "format error! found variable but extract value failed: (%s)", tmp_cell_str.c_str());
				return ::pilo::mk_perr(PERR_INC_DATA);
			}
			if (parts[0].ptr == nullptr || parts[0].length < 2) {
				_compose_errmsg(errmsg, i, 1, "format error! found variable but var name is invalid from - (%s)", tmp_cell_str.c_str());
				return ::pilo::mk_perr(PERR_INC_DATA);
			}
			var_name.assign(parts[0].ptr, parts[0].length);
			::pilo::core::string::to_upper_case_inplace(var_name);

			if (var_name == "$NAME") {
				if (parts[1].length < 1) {
					_compose_errmsg(errmsg, i, 1, "missing content! no value for ($NAME) is found");
					return ::pilo::mk_perr(PERR_INC_DATA);
				}
				_name.assign(parts[1].ptr, parts[1].length);
			}
			else if (var_name == "$DESC") {
				if (parts[1].length > 0) {
					_desc.assign(parts[1].ptr, parts[1].length);
				}				
			}
			else if (var_name == "$CCNAME") {
				if (parts[1].length > 0) {
					this->_configs[xls_config_set::client]._cls_name.assign(parts[1].ptr, parts[1].length);
				}
				else {
					this->_configs[xls_config_set::client]._cls_name = this->_name;
				}				
			}
			else if (var_name == "$SCNAME") {
				if (parts[1].length > 0) {
					this->_configs[xls_config_set::server]._cls_name.assign(parts[1].ptr, parts[1].length);
				}
				else {
					this->_configs[xls_config_set::server]._cls_name = this->_name;
				}
			}
			else if (var_name == "$SCFG") {
				if (parts[1].length > 0) {
					this->_configs[xls_config_set::server]._config_file_name.assign(parts[1].ptr, parts[1].length);
				}
			}
			else if (var_name == "$CCFG") {
				if (parts[1].length > 0) {
					this->_configs[xls_config_set::client]._config_file_name.assign(parts[1].ptr, parts[1].length);
				}
			}
			else if (var_name == "$CNS") {
				if (parts[1].length > 0) {
					this->_configs[xls_config_set::client]._ns.assign(parts[1].ptr, parts[1].length);
				}				
			}
			else if (var_name == "$SNS") {
				if (parts[1].length > 0) {
					this->_configs[xls_config_set::server]._ns.assign(parts[1].ptr, parts[1].length);
				}				
			}
			else if (var_name == "$CUI") {
				if (parts[1].length > 0) {
					err = this->_configs[xls_config_set::client].parse_union_index(parts[1].ptr, parts[1].length);
					if (err != PILO_OK) {
						_compose_errmsg(errmsg, i, 1, "parser UI Key Failed!");
						return ::pilo::mk_perr(PERR_INC_DATA);
					}
				}
			}
			else if (var_name == "$SUI") {
				if (parts[1].length > 0) {
					err = this->_configs[xls_config_set::server].parse_union_index(parts[1].ptr, parts[1].length);
					if (err != PILO_OK) {
						_compose_errmsg(errmsg, i, 1, "parser UI Key Failed!");
						return ::pilo::mk_perr(PERR_INC_DATA);
					}
				}
			}
		}
		else {
			if (!content_start) {
				if (_sheet_name.empty()) {
					_compose_errmsg(errmsg, i, 1, "Got Content (%s) but ($NAME) is not assigned", tmp_cell_str.c_str());
					return ::pilo::mk_perr(PERR_INC_DATA);
				}
				if (_configs[xls_config_set::client].cls_name().empty()) {
					_compose_errmsg(errmsg, i, 1, "Got Content (%s) but client-($CCNAME) is assigned", tmp_cell_str.c_str());
					return ::pilo::mk_perr(PERR_INC_DATA);
				}
				if (_configs[xls_config_set::server].cls_name().empty()) {
					_compose_errmsg(errmsg, i, 1, "Got Content (%s) but server-($CCNAME) is assigned", tmp_cell_str.c_str());
					return ::pilo::mk_perr(PERR_INC_DATA);
				}

				::pilo::u32_t ccnt = xss.col_count();
				if (ccnt < 1) {
					_compose_errmsg(errmsg, i, 1, "Invalid Column Count (%u)", ccnt);
					return ::pilo::mk_perr(PERR_INC_DATA);
				}

				std::string tmp_fld_str;
				std::string tmp_fld_str2;
				for (::pilo::u32_t fld_no = 1; fld_no <= ccnt; fld_no++) {
					err = xss.strvalue(tmp_fld_str, i, fld_no);
					if (err != PILO_OK)
						return err;
					::pilo::core::string::trim_string(tmp_fld_str);
					if (!tmp_fld_str.empty() && tmp_fld_str[0] != '#') {
						err = this->_parse_field(xls_config_set::server,tmp_fld_str, i, fld_no);
						if (err != PILO_OK)
							return err;
					}
				}
				i++;
				for (::pilo::u32_t fld_no = 1; fld_no <= ccnt; fld_no++) {
					err = xss.strvalue(tmp_fld_str, i, fld_no);
					if (err != PILO_OK)
						return err;
					::pilo::core::string::trim_string(tmp_fld_str);
					if (!tmp_fld_str.empty() && tmp_fld_str[0] != '#') {
						err = this->_parse_field(xls_config_set::client, tmp_fld_str, i, fld_no);
						if (err != PILO_OK)
							return err;
					}
				}

				content_start = true;
			}
			else {				
				err = _parse_record(xls_config_set::server, i, &xss, errmsg);
				if (err != PILO_OK) {
					_compose_errmsg(errmsg, i, 1, "parser Record for server Failed!");
					return ::pilo::mk_perr(PERR_INC_DATA);
				}

				err = _parse_record(xls_config_set::client, i, &xss, errmsg);
				if (err != PILO_OK) {
					_compose_errmsg(errmsg, i, 1, "parser Record for client Failed!");
					return ::pilo::mk_perr(PERR_INC_DATA);
				}
			}
		}

	} //loop all lines


	*/
	
	return PILO_OK;
}

::pilo::err_t pilo::core::config::xls_config_set::save_config_file(int which, const char* path_str, predefined_pilo_path prefix)
{
	return _configs[which].save_config_file(path_str, prefix);
}

std::string pilo::core::config::xls_config_set::to_string() const
{
	std::stringstream ss;
	ss << _name << '(' << _sheet_name << ") " << std::endl;
	ss << "Server:" << std::endl;
	ss << _configs[xls_config_set::server].to_string();
	ss << "Client:" << std::endl;
	ss << _configs[xls_config_set::client].to_string();


	return ss.str();
}

void pilo::core::config::xls_config_set::_compose_errmsg(std::string& errmsg, ::pilo::u32_t row, ::pilo::u32_t col, const char* fmt, ...)
{
	char buf[512] = { 0 };
	::pilo::i64_t elen = ::pilo::core::io::string_formated_output(buf, sizeof(buf), "@ [%03u:%02u] -> ", row, col);
	::pilo::i64_t remain_capa = sizeof(buf) - elen;

	va_list args;

	va_start(args, fmt);
#               if defined(WINDOWS)
	_vsnprintf_s(buf + elen, remain_capa, _TRUNCATE, fmt, args);
#               else
	vsnprintf(buf + elen, remain_capa, fmt, args);
#               endif	
	va_end(args);
	errmsg = buf;
}

::pilo::err_t pilo::core::config::xls_config_set::_parse_field(int which, const std::string& field_str, ::pilo::u32_t row, ::pilo::u32_t col)
{
	std::string errmsg;

	::pilo::cstr_ref<char>	raw_fields[5];
	::pilo::i64_t rcnt = ::pilo::core::string::split_fixed(field_str.c_str(), field_str.size(), ":", 1, raw_fields, 5, false, false, true, true);
	if (rcnt < 2) {
		_compose_errmsg(errmsg, row, col, "Parse Field Failed: Field (%s) too few parts", field_str.c_str());
		return ::pilo::mk_perr(PERR_INC_DATA);
	}
	::pilo::u8_t wt;
	::pilo::u8_t kt;
	::pilo::u16_t vt;
	::pilo::err_t err = ::pilo::core::rtti::wired_type::wired_type::s_parse_cstr_type(wt, kt, vt, raw_fields[1].ptr, raw_fields[1].length);
	if (err != PILO_OK) {
		_compose_errmsg(errmsg, row, col, "Parse Field Failed: Field (%s) Invalid type Specifier.", field_str.c_str());
		return ::pilo::mk_perr(PERR_INC_DATA);
	}
	::pilo::core::config::xls_config_field f;
	if (raw_fields[0].ptr != nullptr && raw_fields[0].length > 0) {
		f._name.assign(raw_fields[0].ptr, raw_fields[0].length);
		f._index = (::pilo::i32_t) this->_configs[which]._fields.size();
		f._column = col;
		f._wired_type.set_wrapper_type(wt);
		f._wired_type.set_value_type(vt);
		f._wired_type.set_key_type(kt);	
		if (rcnt > 2 && raw_fields[2].length > 0) {
			for (int k = 0; k < raw_fields[2].length; k++) {
				if (raw_fields[2].ptr[k] == 'p') {
					f._flags.mark_value(xls_config_set::flag_primary_key);
				} 
				else if (raw_fields[2].ptr[k] == 'n') {
					f._flags.mark_value(xls_config_set::flag_nullable);
				}
				else if (raw_fields[2].ptr[k] == 'i') {
					f._flags.mark_value(xls_config_set::flag_index);
				}
				else if (raw_fields[2].ptr[k] == 'u') {
					f._flags.mark_value(xls_config_set::flag_unique);
				}
				else
				{
					_compose_errmsg(errmsg, row, col, "Parse Field Failed: Field (%s) Invalid Flag Specifier (%c).", field_str.c_str(), raw_fields[2].ptr[k]);
					return ::pilo::mk_perr(PERR_INC_DATA);
				}
			}
		}
		if (rcnt > 3 && raw_fields[3].length > 0) {
			f._default_value_str.assign(raw_fields[3].ptr, raw_fields[3].length);
		}	
		this->_configs[which]._fields.push_back(f);
	}

	return PILO_OK;
}

::pilo::err_t pilo::core::config::xls_config_set::_parse_record(int which, ::pilo::u32_t row, ::pilo::core::dp::xls_spread_sheet* xssp, std::string& errmsg)
{
	::pilo::tlv* record = PILO_CONTEXT->allocate_tlv();
	record->set_dict_type(::pilo::core::rtti::wired_type::key_type_str, ::pilo::core::rtti::wired_type::value_type_tlv);

	for (::pilo::u32_t i = 0; i < _configs[which].field_count(); i++) {
		::pilo::u32_t colno = _configs[which]._fields.at(i).column();
		::pilo::tlv* val_ptr = xssp->value(row, colno);
		if (val_ptr == nullptr) {
			PILO_CONTEXT->deallocate_tlv(record);
			_compose_errmsg(errmsg, row, colno, "Extract data from cell failed");
			return ::pilo::mk_perr(PERR_INC_DATA);
		}
		record->insert<std::string, ::pilo::tlv*>(_configs[which]._fields.at(i).name(), val_ptr, false);

	}
	this->_configs[which]._data->push_back(record, -1, true);

	return PILO_OK;
}

std::string pilo::core::config::xls_config_field::to_string() const
{
	char buffer[32] = { 0 };
	char wtbuf[16] = { 0 };
	::pilo::core::rtti::wired_type::s_to_wired_type_cstr(wtbuf, sizeof(wtbuf), _wired_type.wrapper_type(), _wired_type.key_type(), _wired_type.value_type());
	int i = 0;
	char attrbuf[8] = { 0 };
	if (_flags.test_value(xls_config_set::flag_primary_key)) attrbuf[i++] = 'p';
	if (_flags.test_value(xls_config_set::flag_unique)) attrbuf[i++] = 'u';
	if (_flags.test_value(xls_config_set::flag_index)) attrbuf[i++] = 'i';
	if (_flags.test_value(xls_config_set::flag_nullable)) attrbuf[i++] = 'n';
	::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "%s:%s:%s:%s", _name.c_str(), wtbuf, attrbuf, _default_value_str.c_str());
	return std::string(buffer);
}



::pilo::err_t pilo::core::config::xls_config_generator::s_xls_file_iter_func(::pilo::i8_t event_type, const::pilo::core::io::path* src_path, ::pilo::i8_t fsnt, ::pilo::i32_t layer_idx, ::pilo::i32_t file_idx, void* ctx)
{
	PMC_UNUSED(event_type);
	PMC_UNUSED(fsnt);
	PMC_UNUSED(layer_idx);
	PMC_UNUSED(file_idx);
	pilo::core::config::xls_config_generator* pgen = (pilo::core::config::xls_config_generator*)ctx;
	if (pgen == nullptr) {
		return PERR_NULL_PTR;
	}

	if ((::pilo::core::string::i_compare(src_path->extname(), 0, "xlsx", 0, -1))
		&& (::pilo::core::string::i_compare(src_path->extname(), 0, "xls", 0, -1))) {
		pgen->add_log(::pilo::core::logging::level::info, "File - [%s] Ignored", src_path->fullpath());
		return PILO_OK;
	}

	::pilo::pathlen_t rlen = 0;
	if (src_path->basename(rlen)[0] == '~') {
		pgen->add_log(::pilo::core::logging::level::info, "File - [%s] Ignored", src_path->fullpath());
		return PILO_OK;
	}

	if (fsnt != ::pilo::core::io::path::fs_node_type_file) {
		pgen->add_log(::pilo::core::logging::level::info, "File - [%s] Ignored", src_path->fullpath());
		return PILO_OK;
	}

	pgen->add_log(::pilo::core::logging::level::info, "Parsing File - [%s]", src_path->fullpath());

	::pilo::core::dp::xls_spread_document doc;
	::pilo::err_t eret = doc.open(src_path->fullpath(), ::pilo::core::io::creation_mode::open_existing, ::pilo::predefined_pilo_path::count);
	if (eret != PILO_OK) {
		::pilo::core::io::string_formated_output(pgen->_errmsg_buffer, PMI_XLS_GEN_ERR_BUFF_SIZE, "Error: xls_conf_gen@0x%p Open file <%s> Failed", pgen, src_path->fullpath());
		return eret;
	}

	std::vector<std::string> name_lists;
	eret = doc.get_all_worksheet_names(name_lists);
	if (eret != PILO_OK) {
		::pilo::core::io::string_formated_output(pgen->_errmsg_buffer, PMI_XLS_GEN_ERR_BUFF_SIZE, "Error: xls_conf_gen@0x%p get worksheets name list of <%s> Failed", pgen, src_path->fullpath());
		return eret;
	}

	for (size_t i = 0; i < name_lists.size(); i++) {
		if (name_lists.at(i).size() < 2 || name_lists.at(i)[0] != '$') {
			continue;
		}

		::pilo::core::dp::xls_spread_sheet ws = doc.worksheet_by_index((::pilo::u32_t)(i + 1));

		pgen->add_log(::pilo::core::logging::level::info, "Parsing Sheet [%s] in File - [%s]", ws.name().c_str(), src_path->fullpath());


	}

	return PILO_OK;
}

::pilo::err_t pilo::core::config::xls_config_generator::set(const char* xls_dir_path, ::pilo::predefined_pilo_path xls_dir_path_base
	, const char* dest_server_config_dir_path, ::pilo::predefined_pilo_path dest_server_config_dir_path_base
	, const char* dest_client_config_dir_path, ::pilo::predefined_pilo_path dest_client_config_dir_path_base
	, const char* dest_server_source_dir_path, ::pilo::predefined_pilo_path dest_server_source_dir_path_base
	, const char* dest_client_source_dir_path, ::pilo::predefined_pilo_path dest_client_source_dir_path_base)
{
	if (_xls_dir_path.fullpath() != nullptr) {
		::pilo::core::io::string_formated_output(_errmsg_buffer, PMI_XLS_GEN_ERR_BUFF_SIZE, "Error: xls_conf_gen@0x%p Already set!", this);
		return PERR_EXIST;
	}

	if (nullptr == xls_dir_path) {
		::pilo::core::io::string_formated_output(_errmsg_buffer, PMI_XLS_GEN_ERR_BUFF_SIZE, "Error: xls_conf_gen@0x%p xls dir path is null", this);
		return PERR_NULL_PATH;
	}
	else
		_xls_dir_path.set(xls_dir_path, xls_dir_path_base);

	if (nullptr == dest_server_config_dir_path)
		_dest_server_config_dir_path.reset();
	else
		_dest_server_config_dir_path.set(dest_server_config_dir_path, dest_server_config_dir_path_base);

	if (nullptr == dest_client_config_dir_path)
		_dest_client_config_dir_path.reset();
	else
		_dest_client_config_dir_path.set(dest_client_config_dir_path, dest_client_config_dir_path_base);

	if (nullptr == dest_server_source_dir_path)
		_dest_server_source_dir_path.reset();
	else
		_dest_server_source_dir_path.set(dest_server_source_dir_path, dest_server_source_dir_path_base);

	if (nullptr == dest_client_source_dir_path)
		_dest_client_source_dir_path.reset();
	else
		_dest_client_source_dir_path.set(dest_client_source_dir_path, dest_client_source_dir_path_base);

	_errmsg_buffer[0] = 0;
	return PILO_OK;
}

void pilo::core::config::xls_config_generator::clear()
{
	_xls_dir_path.reset();
	_dest_server_config_dir_path.reset();
	_dest_client_config_dir_path.reset();
	_dest_server_source_dir_path.reset();
	_dest_client_source_dir_path.reset();

	_errmsg_buffer[0] = { 0 };
}

::pilo::err_t pilo::core::config::xls_config_generator::parse()
{
	this->add_log(::pilo::core::logging::level::info, "Start Parsing dir - [%s]", _xls_dir_path.fullpath());

	::pilo::err_t ret = ::pilo::core::io::path::dfs_travel_path(&_xls_dir_path, s_xls_file_iter_func, this, false, ::pilo::core::io::path::evt_node_visiting);
	if (ret != PILO_OK) {
		::pilo::core::io::string_formated_output(_errmsg_buffer, PMI_XLS_GEN_ERR_BUFF_SIZE, "Error: xls_conf_gen@0x%p travel xls dir <%s> Failed", this, _xls_dir_path.fullpath());
		return ret;
	}

	_errmsg_buffer[0] = { 0 };

	return PILO_OK;
}

::pilo::err_t pilo::core::config::xls_config_generator::generate_server_config()
{
	_errmsg_buffer[0] = { 0 };
	return PILO_OK;
}

::pilo::err_t pilo::core::config::xls_config_generator::generate_client_config()
{
	_errmsg_buffer[0] = { 0 };
	return PILO_OK;
}

::pilo::err_t pilo::core::config::xls_config_generator::generate_server_source()
{
	_errmsg_buffer[0] = { 0 };
	return PILO_OK;
}

::pilo::err_t pilo::core::config::xls_config_generator::generate_client_source()
{
	_errmsg_buffer[0] = { 0 };
	return PILO_OK;
}



void pilo::core::config::xls_config_generator::add_log(::pilo::core::logging::level level, ::pilo::u32_t row, ::pilo::u32_t col, const char* fmt, ...)
{
	char buf[PMI_XLS_GEN_ERR_BUFF_SIZE] = { 0 };
	::pilo::i64_t elen = ::pilo::core::io::string_formated_output(buf, sizeof(buf), "XCG_0x%p => @[%03u:%02u] : ",this,  row, col);
	::pilo::i64_t remain_capa = sizeof(buf) - elen;

	va_list args;

	va_start(args, fmt);
#               if defined(WINDOWS)
	_vsnprintf_s(buf + elen, remain_capa, _TRUNCATE, fmt, args);
#               else
	vsnprintf(buf + elen, remain_capa, fmt, args);
#               endif	
	va_end(args);

	::pilo::core::logging::info_item ii((::pilo::u32_t)_logs.size(), ::pilo::core::logging::representative_type::text, level, 0, ::pilo::core::datetime::timestamp_micro_system(), buf);
	_logs.push_back(ii);
}

void pilo::core::config::xls_config_generator::add_log(::pilo::core::logging::level level, const char* fmt, ...)
{
	char buf[PMI_XLS_GEN_ERR_BUFF_SIZE] = { 0 };
	::pilo::i64_t elen = ::pilo::core::io::string_formated_output(buf, sizeof(buf), "XCG_0x%p => @[NA Pos] : ", this);
	::pilo::i64_t remain_capa = sizeof(buf) - elen;

	va_list args;

	va_start(args, fmt);
#               if defined(WINDOWS)
	_vsnprintf_s(buf + elen, remain_capa, _TRUNCATE, fmt, args);
#               else
	vsnprintf(buf + elen, remain_capa, fmt, args);
#               endif	
	va_end(args);

	::pilo::core::logging::info_item ii((::pilo::u32_t)_logs.size(), ::pilo::core::logging::representative_type::text, level, 0, ::pilo::core::datetime::timestamp_micro_system(), buf);
	_logs.push_back(ii);
}