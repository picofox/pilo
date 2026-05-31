////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                    //
//  .----------------.  .----------------.  .----------------.  .----------------.       Raid boss    //
//  | .--------------. || .--------------. || .--------------. || .--------------. |    Lv.85 缺德猫   //
//  | |   ______     | || |     _____    | || |   _____      | || |     ____     | |     |\.-"-./|    //
//  | |  |_   __ \   | || |    |_   _|   | || |  |_   _|     | || |   .'    `.   | |     \`     `/    //
//  | |    | |__) |  | || |      | |     | || |    | |       | || |  /  .--.  \  | |     |= ^Y^ =|    //
//  | |    |  ___/   | || |      | |     | || |    | |   _   | || |  | |    | |  | |     \__ ^ __/    //
//  | |   _| |_      | || |     _| |_    | || |   _| |__/ |  | || |  \  `- - '/  | |     /`=+o+=`\    //
//  | |  |_____|     | || |    |_____|   | || |  |________|  | || |   `.____.'   | |    |         |   //
//  | |              | || |              | || |              | || |              | |    | (     ) |   //
//  | '--------------' || '--------------' || '--------------' || '--------------' |    (,,)---(,,)   // 
//  '----------------'  '----------------'  '----------------'  '----------------'                    //
//                                                                                                    //  
////////////////////////////////////////////////////////////////////////////////////////////////////////

#include	"xls_config.hpp"
#include	"../dp/xls_spread_sheet.hpp"
#include	<sstream>
#include	<algorithm>
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

pilo::core::config::xls_config& pilo::core::config::xls_config::operator=(xls_config&& rhs) noexcept
{
	if (this != &rhs) {
		_cls_name = std::move(rhs._cls_name);
		_config_file_name = std::move(rhs._config_file_name);
		_ns = std::move(rhs._ns);
		_union_indices = std::move(rhs._union_indices);
		_fields = std::move(rhs._fields);
		_cls_fields_index_map = std::move(rhs._cls_fields_index_map);

		if (this->_data != nullptr) {
			PILO_CONTEXT->deallocate_tlv(_data);
			_data = nullptr;
		}
		_data = rhs._data;
		rhs._data = nullptr;
	}

	return *this;
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
	ss << '\t' << "Cls-Fields:" << std::endl;
	for (size_t i = 0; i < _cls_fields_index_map.size(); i++) {
		ss << "\t\t" << _fields[_cls_fields_index_map[i].second].pri() << " - " << _fields[_cls_fields_index_map[i].second].name() << std::endl;
	}
	ss << std::endl;
	return ss.str();
}


::pilo::i32_t pilo::core::config::xls_config::find_lowest_pri_field() const
{
	::pilo::i32_t pri = -1;
	for (size_t i = 0; i < _fields.size(); i++) {
		if (_fields[i].pri() > pri) {
			pri = _fields[i].pri();
		}
	}
	return pri;
}

::pilo::i32_t pilo::core::config::xls_config::find_existing_field_idx_by_name(const std::string& name) const
{
	for (size_t i = 0; i < _fields.size(); i++) {
		if (_fields.at(i).name() == name) {
			return (::pilo::i32_t)i;
		}		
	}
	return -1;
}

void pilo::core::config::xls_config_set::reset()
{
	for (auto i = 0; i < 2; i++) {
		_configs[i].reset();
	}
	_sheet_name.clear();
	_desc.clear();
}


std::string pilo::core::config::xls_config_set::to_string() const
{
	std::stringstream ss;
	ss << "Server:" << std::endl;
	ss << _configs[xls_config_set::server].to_string();
	ss << "Client:" << std::endl;
	ss << _configs[xls_config_set::client].to_string();


	return ss.str();
}

::pilo::err_t pilo::core::config::xls_config_generator::_parse_field_spec(xls_config_set& cfg_set, int which, const std::string& field_str, ::pilo::u32_t row, ::pilo::u32_t col, const char* xlsfullfilepath, const char* wsnamecstr)
{
	::pilo::core::config::xls_config_field f;
	::pilo::err_t err = PILO_OK;

	::pilo::cstr_ref<char>	raw_fields[5];
	::pilo::i64_t rcnt = ::pilo::core::string::split_fixed(field_str.c_str(), field_str.size(), ":", 1, raw_fields, 5, false, false, true, true);
	if (rcnt < 3) {
		add_log(::pilo::core::logging::level::error, row, col, "Low Count (%lld) Split field spec Failed (%s) in %s.%s", rcnt, field_str.c_str(), xlsfullfilepath, wsnamecstr);
		return ::pilo::mk_perr(PERR_INC_DATA);
	}


	//parse pri
	if (raw_fields[0].is_empty()) {
		f._pri = cfg_set._configs[which].find_lowest_pri_field();
	}
	else {
		err = ::pilo::core::string::string_to_number(f._pri, raw_fields[0].ptr, raw_fields[0].length);
		if (err != PILO_OK) {
			add_log(::pilo::core::logging::level::error, row, col, "Pri to number Failed (%s) in %s.%s", raw_fields[0], xlsfullfilepath, wsnamecstr);
			return ::pilo::mk_perr(PERR_INC_DATA);
		}
	}

	//parse name
	if (raw_fields[1].is_empty()) {
		add_log(::pilo::core::logging::level::error, row, col, "Name is empty in %s.%s", xlsfullfilepath, wsnamecstr);
		return ::pilo::mk_perr(PERR_INC_DATA);
	}
	else {
		f._name.assign(raw_fields[1].ptr, raw_fields[1].length);
	}
	::pilo::i32_t existing_fld_pos = cfg_set._configs[which].find_existing_field_idx_by_name(f.name());
	if (existing_fld_pos >= 0) {
		add_log(::pilo::core::logging::level::error, row, col, "Name (%s) is existing at field_%d in %s.%s", f._name.c_str(), existing_fld_pos, xlsfullfilepath, wsnamecstr);
		return ::pilo::mk_perr(PERR_INC_DATA);
	}

	//parse type
	::pilo::u8_t wt = ::pilo::core::rtti::wired_type::wrapper_na;
	::pilo::u8_t kt = ::pilo::core::rtti::wired_type::key_type_na;
	::pilo::u16_t vt = ::pilo::core::rtti::wired_type::value_type_na;
	std::string type_tmp_str;
	type_tmp_str.assign(raw_fields[2].ptr, raw_fields[2].length);
	err = ::pilo::core::rtti::wired_type::wired_type::s_parse_cstr_type(wt, kt, vt, type_tmp_str.c_str(), (::pilo::i64_t)type_tmp_str.size());
	if (err != PILO_OK) {
		add_log(::pilo::core::logging::level::error, row, col, "Type (%s) parse Failed. in %s.%s", type_tmp_str.c_str(), xlsfullfilepath, wsnamecstr);
		return ::pilo::mk_perr(PERR_INC_DATA);
	}
	if (wt == ::pilo::core::rtti::wired_type::wrapper_single) {
		if (vt == ::pilo::core::rtti::wired_type::value_type_na) {
			add_log(::pilo::core::logging::level::error, row, col, "Type (%s) parse Failed. Got NA type for single type. in %s.%s", type_tmp_str.c_str(), xlsfullfilepath, wsnamecstr);
			return ::pilo::mk_perr(PERR_INC_DATA);
		}
	}
	else if (wt == ::pilo::core::rtti::wired_type::wrapper_array) {
		if (vt == ::pilo::core::rtti::wired_type::value_type_na) {
			add_log(::pilo::core::logging::level::error, row, col, "Type (%s) parse Failed. Got NA type for array type. in %s.%s", type_tmp_str.c_str(), xlsfullfilepath, wsnamecstr);
			return ::pilo::mk_perr(PERR_INC_DATA);
		}
	}
	else if (wt == ::pilo::core::rtti::wired_type::wrapper_dict) {
		if (kt == ::pilo::core::rtti::wired_type::key_type_na) {
			add_log(::pilo::core::logging::level::error, row, col, "Type (%s) parse Failed. Got NA key type for hash type. in %s.%s", type_tmp_str.c_str(), xlsfullfilepath, wsnamecstr);
			return ::pilo::mk_perr(PERR_INC_DATA);
		}
		if (vt == ::pilo::core::rtti::wired_type::value_type_na) {
			add_log(::pilo::core::logging::level::error, row, col, "Type (%s) parse Failed. Got NA val type for hash type in %s.%s", type_tmp_str.c_str(), xlsfullfilepath, wsnamecstr);
			return ::pilo::mk_perr(PERR_INC_DATA);
		}
	}
	f._wired_type.set_wrapper_type(wt);
	f._wired_type.set_value_type(vt);
	f._wired_type.set_key_type(kt);

	//parse attr
	std::string attr_str;
	attr_str.assign(raw_fields[3].ptr, raw_fields[3].length);
	if (rcnt > 3 && attr_str.size() > 0) {
		for (size_t k = 0; k < attr_str.size(); k++) {
			if (attr_str.at(k) == 'p') {
				f._flags.mark_value(xls_config_set::flag_primary_key);
			}
			else if (attr_str.at(k) == 'P') {
				f._flags.mark_value(xls_config_set::flag_primary_key_array);
			}
			else if (attr_str.at(k) == 'n') {
				f._flags.mark_value(xls_config_set::flag_nullable);
			}
			else if (attr_str.at(k) == 'i') {
				f._flags.mark_value(xls_config_set::flag_index);
			}
			else if (attr_str.at(k) == 'u') {
				f._flags.mark_value(xls_config_set::flag_unique);
			}
			else
			{
				add_log(::pilo::core::logging::level::error, row, col, "Attr (%s) parse Failed. Got Invalid marker (%c) for field %s in %s.%s", attr_str.c_str(), attr_str.at(k),  f.name().c_str(), xlsfullfilepath, wsnamecstr);
				return ::pilo::mk_perr(PERR_INC_DATA);
			}
		}
	}
	if (f._flags.test_value(xls_config_set::flag_primary_key) && f._flags.test_value(xls_config_set::flag_primary_key_array)) {
		add_log(::pilo::core::logging::level::error, row, col, "Attr (%s) parse Failed. Got pk and pk_arr both assigned in %s.%s", attr_str.c_str(),  xlsfullfilepath, wsnamecstr);
		return ::pilo::mk_perr(PERR_INC_DATA);
	}

	//default value
	if (rcnt > 4 && raw_fields[4].length > 0) {
		f._default_value_str.assign(raw_fields[4].ptr, raw_fields[4].length);
		if (f._flags.test_value(xls_config_set::flag_nullable)) {
			add_log(::pilo::core::logging::level::warn, row, col, "Default value (%s) Found but attr has no nullable assigned in %s.%s", f._default_value_str.c_str(), xlsfullfilepath, wsnamecstr);
		}
		
	}

	f._index = (::pilo::i32_t)cfg_set._configs[which]._fields.size();
	f._column = col;

	cfg_set._configs[which]._cls_fields_index_map.emplace_back(std::pair<::pilo::i32_t, ::pilo::i32_t>(f._pri, f._index));
	std::stable_sort(cfg_set._configs[which]._cls_fields_index_map.begin(), cfg_set._configs[which]._cls_fields_index_map.end(), [](const auto& a, const auto& b) {
		return a.first < b.first;  
		});

	cfg_set._configs[which]._fields.push_back(std::move(f));
	return PILO_OK;
}

::pilo::err_t pilo::core::config::xls_config_generator::_parse_record(xls_config_set& cfg_set, int which, ::pilo::u32_t row, ::pilo::core::dp::xls_spread_sheet* wsp, const char* xlsfullfilepath, const char* wsnamecstr)
{
	xls_config& cfg_ref = cfg_set._configs[which];
	::pilo::tlv* val_ptr = nullptr;
	::pilo::tlv* record = PILO_CONTEXT->allocate_tlv();
	record->set_dict_type(::pilo::core::rtti::wired_type::key_type_str, ::pilo::core::rtti::wired_type::value_type_tlv);
	::pilo::err_t err = PILO_OK;
	char errmsgbuff[PMI_XLS_GEN_ERR_BUFF_SIZE] = {0};

	for (::pilo::u32_t i = 0; i < cfg_ref.field_count(); i++) {
		::pilo::u32_t colno = cfg_ref._fields.at(i).column();
		val_ptr = PILO_CONTEXT->allocate_tlv();
		if (val_ptr == nullptr) {
			this->add_log(::pilo::core::logging::level::error, row, colno, "Alloc tlv Failed. in %s.%s", xlsfullfilepath, wsnamecstr);
			return mk_err(PERR_INSUF_HEAP);
		}
		err = val_ptr->set_types(cfg_ref._fields.at(i).wrapper_type(), cfg_ref._fields.at(i).key_type(), cfg_ref._fields.at(i).value_type());
		if (PILO_OK != err) {
			PILO_CONTEXT->deallocate_tlv(val_ptr);
			this->add_log(::pilo::core::logging::level::error, row, colno, "Set type for tlv Failed (%s). in %s.%s", ::pilo::str_err(err).c_str(), xlsfullfilepath, wsnamecstr);
			return err;
		}

		err = wsp->value(val_ptr, row, colno, cfg_ref._fields.at(i).test_flag(xls_config_set::flag_nullable), cfg_ref._fields.at(i).default_value(), errmsgbuff, sizeof(errmsgbuff));
		if (PILO_OK != err) {
			PILO_CONTEXT->deallocate_tlv(val_ptr);
			this->add_log(::pilo::core::logging::level::error, row, colno, "Compse tlv from cell Failed (%s). in %s.%s", errmsgbuff, xlsfullfilepath, wsnamecstr);
			return err;
		}

		err = record->insert<std::string, ::pilo::tlv*>(cfg_ref._fields.at(i).name(), val_ptr, false);
		if (PILO_OK != err) {
			PILO_CONTEXT->deallocate_tlv(val_ptr);
			this->add_log(::pilo::core::logging::level::error, row, colno, "Insert tlv to record Failed (%s). in %s.%s", ::pilo::str_err(err).c_str(), xlsfullfilepath, wsnamecstr);
			return err;
		}
	}
	cfg_ref._data->push_back(record, -1, true);

	return PILO_OK;
}

std::string pilo::core::config::xls_config_field::to_string() const
{
	char buffer[256] = { 0 };
	char wtbuf[16] = { 0 };
	::pilo::core::rtti::wired_type::s_to_wired_type_cstr(wtbuf, sizeof(wtbuf), _wired_type.wrapper_type(), _wired_type.key_type(), _wired_type.value_type());
	int i = 0;
	char attrbuf[8] = { 0 };
	if (_flags.test_value(xls_config_set::flag_primary_key)) attrbuf[i++] = 'p';
	if (_flags.test_value(xls_config_set::flag_unique)) attrbuf[i++] = 'u';
	if (_flags.test_value(xls_config_set::flag_index)) attrbuf[i++] = 'i';
	if (_flags.test_value(xls_config_set::flag_nullable)) attrbuf[i++] = 'n';
	if (_flags.test_value(xls_config_set::flag_primary_key_array)) attrbuf[i++] = 'P';
	::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "%d: %d:%u:%s:%s:%s:%s",_index, _pri,_column, _name.c_str(), wtbuf, attrbuf, _default_value_str.c_str());
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

	if (src_path->extname() == nullptr) {
		pgen->add_log(::pilo::core::logging::level::info, "File - [%s] Ignored", src_path->fullpath());
		return PILO_OK;
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

	::pilo::err_t eret = pgen->_parse_xls(src_path->fullpath());
	if (eret != PILO_OK) {
		return eret;
	}
	

	return PILO_OK;
}

::pilo::err_t pilo::core::config::xls_config_generator::_parse_xls(const char* xls_fullpath)
{
	this->add_log(::pilo::core::logging::level::info, "Parsing File - [%s]", xls_fullpath);

	::pilo::core::dp::xls_spread_document doc;
	::pilo::err_t eret = doc.open(xls_fullpath, ::pilo::core::io::creation_mode::open_existing, ::pilo::predefined_pilo_path::count);
	if (eret != PILO_OK) {
		this->add_log(::pilo::core::logging::level::error, "xls_conf_gen@0x%p Open file <%s> Failed", this, xls_fullpath);
		return eret;
	}

	std::vector<std::string> name_lists;
	eret = doc.get_all_worksheet_names(name_lists);
	if (eret != PILO_OK) {
		this->add_log(::pilo::core::logging::level::error, "xls_conf_gen@0x%p get worksheets name list of <%s> Failed", this, xls_fullpath);
		return eret;
	}

	for (size_t i = 0; i < name_lists.size(); i++) {
		if (name_lists.at(i).size() < 2 || name_lists.at(i)[0] != '$') {
			continue;
		}

		if ((eret = _parse_worksheet(doc, xls_fullpath, (::pilo::u32_t)(i + 1))) != PILO_OK) {
			return eret;
		}
	}

	return PILO_OK;
}


bool pilo::core::config::xls_config_generator::_check_and_make_default_for_two_vars(const char* t1, const char* t2, std::string& a, std::string& b,  const char* file, const char* wsname)
{
	if (a.size() > 0 && b.size() > 0) {		
		return true;
	} else if (a.empty()) {
		add_log(::pilo::core::logging::level::warn, "Warn: var of %s not found in %s.%s, fallback to %s (%s) as its value", t1,  file, wsname, t2, b.c_str());
		a = b;
		return true;
	} else if (b.empty()) {
		add_log(::pilo::core::logging::level::warn, "Warn: var of %s not found in %s.%s, fallback to %s (%s) as its value", t2, file, wsname, t1, a.c_str());
		b = a;
		return true;
	}
	this->add_log(::pilo::core::logging::level::warn, "value for both %s & %s of %s.%s not found", t1, t2, file, wsname);
	return false;
}

bool pilo::core::config::xls_config_generator::_check_duplicate_vars_in_header(const std::string& ccname, const std::string& scname, const std::string& ccfg, const std::string& scfg, const char* file, const char* wsname)
{
	std::map<std::string, std::string>::const_iterator cit = _ccnames.find(ccname);
	if (cit != _ccnames.cend()) {
		this->add_log(::pilo::core::logging::level::error, "Duplicate CCNAME (%s) in %s.%s and %s", ccname.c_str(), file, wsname, cit->second.c_str());
		return false;
	}

	cit = _scnames.find(scname);
	if (cit != _scnames.cend()) {
		this->add_log(::pilo::core::logging::level::error, "Duplicate SCNAME (%s) in %s.%s and %s", scname.c_str(), file, wsname, cit->second.c_str());
		return false;
	}

	cit = _ccfgs.find(ccfg);
	if (cit != _ccfgs.cend()) {
		this->add_log(::pilo::core::logging::level::error, "Duplicate CCFG (%s) in %s.%s and %s", ccfg.c_str(), file, wsname, cit->second.c_str());
		return false;
	}

	cit = _scfgs.find(scfg);
	if (cit != _scfgs.cend()) {
		this->add_log(::pilo::core::logging::level::error, "Duplicate SCFG (%s) in %s.%s and %s", scfg.c_str(), file, wsname, cit->second.c_str());
		return false;
	}
	
	return true;
}

::pilo::err_t pilo::core::config::xls_config_generator::_generate_config(int which, const char* name_of_which)
{
	::pilo::core::io::path dst_path;
	::pilo::err_t err = PILO_OK;
	std::map<std::string, xls_config_set>::const_iterator cit = _config_set_map.cbegin();

	err = _dest_config_dir_path[which].create(::pilo::core::io::path::fs_node_type_dir, false);	
	if (err != PILO_OK) {
		this->add_log(::pilo::core::logging::level::error, "%s Config File Dest Path [%s] create Failed.",name_of_which, _dest_config_dir_path[which].fullpath());
		return mk_err(err);
	}

	for (; cit != _config_set_map.cend(); cit++) {
		dst_path = _dest_config_dir_path[which];
		err = dst_path.append(cit->second._configs[which].config_file_name().c_str());
		if (err != PILO_OK) {
			this->add_log(::pilo::core::logging::level::error, "%s Config File generated failed. dest path compose Failed. append %s -> %s", name_of_which, cit->second._configs[xls_config_set::server].config_file_name().c_str(), dst_path.fullpath());
			return mk_err(err);
		}

		::pilo::core::ml::json_tlv_driver jdrv(cit->second._configs[which]._data);
		err = jdrv.save(&dst_path);
		if (err != PILO_OK) {
			this->add_log(::pilo::core::logging::level::error, "%s Config File generated failed. [%s] Serialize to Json Failedd %d ", name_of_which, cit->second._configs[xls_config_set::server].config_file_name().c_str(), err);
			return mk_err(err);
		}

		this->add_log(::pilo::core::logging::level::info, "%s Config File [%s] has been created successfully.", name_of_which, dst_path.fullpath());

	}

	return PILO_OK;
}

::pilo::err_t pilo::core::config::xls_config_generator::_parse_worksheet(::pilo::core::dp::xls_spread_document& doc, const char* xls_fullpathname, ::pilo::u32_t ws_idx)
{	
	::pilo::core::dp::xls_spread_sheet ws = doc.worksheet_by_index(ws_idx);
	std::string wsname = ws.name();
	add_log(::pilo::core::logging::level::info, "Parsing Sheet [%s] in File - [%s]", wsname.c_str(), xls_fullpathname);

	std::string tmp_cell_str, tmp_varname_cell;
	::pilo::err_t err = PILO_OK;
	::pilo::u32_t rcnt = ws.row_count();
	bool c_header_done = false;
	bool s_header_done = false;

	std::string strkvpair[2];
	std::string desc, ccname, scname, ccfg, scfg, cns, sns, cui, sui;

	std::string filepathname_n_ws = xls_fullpathname;
	filepathname_n_ws += '.';
	filepathname_n_ws += wsname;

	if (_config_set_map.find(filepathname_n_ws) != _config_set_map.cend()) {
		this->add_log(::pilo::core::logging::level::error, "Duplicate xls file and sheet exists. %s.%s", xls_fullpathname, wsname.c_str());
		return err;
	}

	if (rcnt < 2 || rcnt == ::pilo::core::dp::xls_spread_sheet::invalid_count) {
		return ::pilo::mk_perr(PERR_INC_DATA);
	}

	xls_config_set cfg_set;

	xls_config_generator::parse_phase_enum pe = xls_config_generator::parse_phase_enum::header_vars;
	for (::pilo::u32_t r = 1; r <= rcnt; r++) {	
		tmp_cell_str.clear();
		tmp_varname_cell.clear();
		strkvpair[0].clear();
		strkvpair[1].clear();
		std::string tmp_fld_string;

		err = ws.strvalue(tmp_cell_str, r, 1);
		::pilo::core::string::trim_string(tmp_cell_str);
		if (err != PILO_OK) {
			this->add_log(::pilo::core::logging::level::error, "Read instruct of %s.%s cell @[%03u:%02u] Failed.", xls_fullpathname, wsname.c_str(), r, 1);
			return err;
		}

		if (tmp_cell_str.size() == 1 && tmp_cell_str.at(0) == '$') {
			if (xls_config_generator::parse_phase_enum::col_spec == pe) {
				this->add_log(::pilo::core::logging::level::error, "Got var instruct section at %s.%s @[%03u:%02u], phase is %d ", xls_fullpathname, wsname.c_str(), r, 1, (int)pe);
				return err;
			} else if (xls_config_generator::parse_phase_enum::row_data == pe) {
				err = ws.strvalue(tmp_varname_cell, r, 2);
				::pilo::core::string::trim_string(tmp_varname_cell);
				if (::pilo::core::string::i_compare(tmp_varname_cell.c_str(), 0, "END", 0, -1) == 0) {
					add_log(::pilo::core::logging::level::info, r, 2, "Read data finished: %s.%s.", xls_fullpathname, wsname.c_str());
					break;
				} else {
					this->add_log(::pilo::core::logging::level::error, "Got instruct at %s.%s @[%03u:%02u], phase is %d, but cmd is not /END/.", xls_fullpathname, wsname.c_str(), r, 1, (int)pe);
					return err;
				}						
			}


			err = ws.strvalue(tmp_varname_cell, r, 2);
			::pilo::core::string::trim_string(tmp_varname_cell);
			if (err != PILO_OK) {
				this->add_log(::pilo::core::logging::level::error, "Read var of %s.%s cell @[%03u:%02u] Failed.", xls_fullpathname, wsname.c_str(), r, 2);
				return err;
			}
			::pilo::i64_t idx_tmp = ::pilo::core::string::split(strkvpair, 2, tmp_varname_cell, ':', true);
			if (idx_tmp < 1 || idx_tmp > 2) {
				this->add_log(::pilo::core::logging::level::error, "Format of var of %s.%s cell @[%03u:%02u] Failed. shoudl be /key:value/", xls_fullpathname, wsname.c_str(), r, 2);
				return err;
			}

			if (::pilo::core::string::i_compare(strkvpair[0].c_str(), 0, "DESC", 0, -1) == 0) {
				desc = strkvpair[1];				
			} else if (::pilo::core::string::i_compare(strkvpair[0].c_str(), 0, "CCNAME", 0, -1) == 0) {
				ccname = strkvpair[1];
				if (ccname.empty()) {
					this->add_log(::pilo::core::logging::level::warn, "CCNAME (%s) not set, client class file wont be generated for %s.%s", ccname.c_str(), xls_fullpathname, wsname.c_str());
				}
			} else if (::pilo::core::string::i_compare(strkvpair[0].c_str(), 0, "SCNAME", 0, -1) == 0) {
				scname = strkvpair[1];
				if (scname.empty()) {
					this->add_log(::pilo::core::logging::level::warn, "SCNAME (%s) not set, server class file wont be generated for %s.%s", scname.c_str(), xls_fullpathname, wsname.c_str());
				}
			} else if (::pilo::core::string::i_compare(strkvpair[0].c_str(), 0, "CCFG", 0, -1) == 0) {
				ccfg = strkvpair[1];
				ccname = strkvpair[1];
				if (ccname.empty()) {
					this->add_log(::pilo::core::logging::level::warn, "CCFG (%s) not set, client data file wont be generated for %s.%s", ccfg.c_str(), xls_fullpathname, wsname.c_str());
				}
			} else if (::pilo::core::string::i_compare(strkvpair[0].c_str(), 0, "SCFG", 0, -1) == 0) {
				scfg = strkvpair[1];
				ccname = strkvpair[1];
				if (ccname.empty()) {
					this->add_log(::pilo::core::logging::level::warn, "SCFG (%s) not set, server data file wont be generated for %s.%s", scfg.c_str(), xls_fullpathname, wsname.c_str());
				}
			} else if (::pilo::core::string::i_compare(strkvpair[0].c_str(), 0, "CNS", 0, -1) == 0) {
				cns = strkvpair[1];
			} else if (::pilo::core::string::i_compare(strkvpair[0].c_str(), 0, "SNS", 0, -1) == 0) {
				sns = strkvpair[1];
			} else if (::pilo::core::string::i_compare(strkvpair[0].c_str(), 0, "CUI", 0, -1) == 0) {
				cui = strkvpair[1];
			} else if (::pilo::core::string::i_compare(strkvpair[0].c_str(), 0, "SUI", 0, -1) == 0) {
				sui = strkvpair[1];
			} else {
				this->add_log(::pilo::core::logging::level::error, "Varname of %s.%s cell @[%03u:%02u] not found.", xls_fullpathname, wsname.c_str(), r, 2);
				return PERR_INC_DATA;
			}
			
		} else if (tmp_cell_str.size() == 1 && (tmp_cell_str.at(0) == 'C' || tmp_cell_str.at(0) == 'c')) {
			if (xls_config_generator::parse_phase_enum::row_data == pe) {
				this->add_log(::pilo::core::logging::level::error, "Got client-col-spec instruct section at %s.%s @[%03u:%02u], phase is %d ", xls_fullpathname, wsname.c_str(), r, 1, (int)pe);
				return PERR_INC_DATA;
			} else if (xls_config_generator::parse_phase_enum::header_vars == pe) {
				if (!_check_duplicate_vars_in_header(ccname, scname, ccfg, scfg, xls_fullpathname, wsname.c_str()))
					return PERR_INC_DATA;
				pe = xls_config_generator::parse_phase_enum::col_spec;				
			}

			::pilo::u32_t field_tmp_cnt = ws.col_count();
			if (field_tmp_cnt < 1) {
				this->add_log(::pilo::core::logging::level::error, "Client Fields Spec Got 0 or minus Count (row=%u) of %s.%s", r, xls_fullpathname, wsname.c_str());
				return ::pilo::mk_perr(PERR_INC_DATA);
			}
			::pilo::u32_t field_real_cnt = 0;			
			for (::pilo::u32_t c = 2; c <= field_tmp_cnt; c++) {				
				err = ws.strvalue(tmp_fld_string, r, c);
				if (err != PILO_OK) {
					this->add_log(::pilo::core::logging::level::error, "Client Fields Spec Read Failed @[%03u:%02u]  of %s.%s", r, c, xls_fullpathname, wsname.c_str());
					return ::pilo::mk_perr(PERR_INC_DATA);
				}
				if (tmp_fld_string.empty()) {
					break;
				}
				::pilo::core::string::trim_string(tmp_fld_string);
				if (tmp_fld_string.size() > 1 && tmp_fld_string.at(0) == '/' && tmp_fld_string.at(1) == '/')
					continue;
				if ((err = this->_parse_field_spec(cfg_set, xls_config_set::client, tmp_fld_string, r, c, xls_fullpathname, wsname.c_str())) != PILO_OK) {
					return err;
				}
				
				
				field_real_cnt++;
			}

			if ((::pilo::u32_t) cfg_set._configs[xls_config_set::client]._fields.size() != field_real_cnt) {
				this->add_log(::pilo::core::logging::level::error, "Client Fields Spec Count Mismatch %u : %u,  %s.%s", (::pilo::u32_t) cfg_set._configs[xls_config_set::client]._fields.size() , field_real_cnt, xls_fullpathname, wsname.c_str());
				return PERR_INC_DATA;
			}
			
			
			if (s_header_done)
				pe = xls_config_generator::parse_phase_enum::row_data;
			else 
				c_header_done = true;			

		} else if (tmp_cell_str.size() == 1 && (tmp_cell_str.at(0) == 'S' || tmp_cell_str.at(0) == 's')) {
			if (xls_config_generator::parse_phase_enum::row_data == pe) {
				this->add_log(::pilo::core::logging::level::error, "Got server-col-spec instruct section at %s.%s @[%03u:%02u], phase is %d ", xls_fullpathname, wsname.c_str(), r, 1, (int)pe);
				return PERR_INC_DATA;
			} else if (xls_config_generator::parse_phase_enum::header_vars == pe) {
				if (!_check_duplicate_vars_in_header(ccname, scname, ccfg, scfg, xls_fullpathname, wsname.c_str()))
					return PERR_INC_DATA;
				pe = xls_config_generator::parse_phase_enum::col_spec;

			}
			::pilo::u32_t field_tmp_cnt = ws.col_count();
			if (field_tmp_cnt < 1) {
				this->add_log(::pilo::core::logging::level::error, "Server Fields Spec Got 0 or minus Count (row=%u) of %s.%s", r, xls_fullpathname, wsname.c_str());
				return ::pilo::mk_perr(PERR_INC_DATA);
			}

			::pilo::u32_t field_real_cnt = 0;
			for (::pilo::u32_t c = 2; c <= field_tmp_cnt; c++) {
				err = ws.strvalue(tmp_fld_string, r, c);
				if (err != PILO_OK) {
					this->add_log(::pilo::core::logging::level::error, "Server Fields Spec Read Failed @[%03u:%02u]  of %s.%s", r, c, xls_fullpathname, wsname.c_str());
					return ::pilo::mk_perr(PERR_INC_DATA);
				}
				if (tmp_fld_string.empty()) {
					break;
				}
				::pilo::core::string::trim_string(tmp_fld_string);
				if (tmp_fld_string.size() > 1 && tmp_fld_string.at(0) == '/' && tmp_fld_string.at(1) == '/')
					continue;
				if ((err = this->_parse_field_spec(cfg_set, xls_config_set::server, tmp_fld_string, r, c, xls_fullpathname, wsname.c_str())) != PILO_OK) {
					return err;
				}


				field_real_cnt++;
			}

			if ((::pilo::u32_t)cfg_set._configs[xls_config_set::server]._fields.size() != field_real_cnt) {
				this->add_log(::pilo::core::logging::level::error, "Server Fields Spec Count Mismatch %u : %u,  %s.%s", (::pilo::u32_t)cfg_set._configs[xls_config_set::server]._fields.size(), field_real_cnt, xls_fullpathname, wsname.c_str());
				return PERR_INC_DATA;
			}

			if (c_header_done) 
				pe = xls_config_generator::parse_phase_enum::row_data;			
			else
				s_header_done = true;	

		}
		else if (tmp_cell_str.size() > 0 && tmp_cell_str.at(0) == '/' && tmp_cell_str.at(1) == '/') {
			continue;
		} else if (tmp_cell_str.empty()) {
			if (xls_config_generator::parse_phase_enum::row_data != pe) {
				this->add_log(::pilo::core::logging::level::error, "Got empty instruct section at %s.%s @[%03u:%02u], phase is %d ", xls_fullpathname, wsname.c_str(), r, 1, (int)pe);
				return err;
			}

			err = this->_parse_record(cfg_set, xls_config_set::server, r, &ws, xls_fullpathname, wsname.c_str());
			if (err != PILO_OK) {
				return err;
			}

		}

		
			
		
	} //end of while


	cfg_set._desc = desc;
	cfg_set._sheet_name = wsname;
	cfg_set._xls_name = xls_fullpathname;
	cfg_set._configs[xls_config_set::server]._cls_name = scname;
	cfg_set._configs[xls_config_set::server]._config_file_name = scfg;
	cfg_set._configs[xls_config_set::server]._ns = sns;
	cfg_set._configs[xls_config_set::client]._cls_name = ccname;
	cfg_set._configs[xls_config_set::client]._config_file_name = ccfg;
	cfg_set._configs[xls_config_set::client]._ns = cns;
	cfg_set._configs[xls_config_set::server].parse_union_index(sui.c_str(), (pilo::i64_t)sui.size());
	cfg_set._configs[xls_config_set::client].parse_union_index(cui.c_str(), (pilo::i64_t)cui.size());

	
	if (! ccname.empty())
		_ccnames.insert(std::pair<std::string, std::string>(ccname, filepathname_n_ws));
	if (! scname.empty())
		_scnames.insert(std::pair<std::string, std::string>(scname, filepathname_n_ws));
	if (! ccfg.empty())
		_ccfgs.insert(std::pair<std::string, std::string>(ccfg, filepathname_n_ws));
	if (! scfg.empty())
		_scfgs.insert(std::pair<std::string, std::string>(scfg, filepathname_n_ws));

	_config_set_map.insert(std::pair<std::string, xls_config_set>(filepathname_n_ws, std::move(cfg_set)));

	return PILO_OK;
}


::pilo::err_t pilo::core::config::xls_config_generator::set(const char* xls_dir_path, ::pilo::predefined_pilo_path xls_dir_path_base
	, const char* dest_server_config_dir_path, ::pilo::predefined_pilo_path dest_server_config_dir_path_base
	, const char* dest_client_config_dir_path, ::pilo::predefined_pilo_path dest_client_config_dir_path_base
	, const char* dest_server_source_dir_path, ::pilo::predefined_pilo_path dest_server_source_dir_path_base
	, const char* dest_client_source_dir_path, ::pilo::predefined_pilo_path dest_client_source_dir_path_base)
{
	this->clear();

	if (_xls_dir_path.fullpath() != nullptr) {
		this->add_log(::pilo::core::logging::level::error, "xls_conf_gen@0x%p Already set!", this);
		return PERR_EXIST;
	}

	if (nullptr == xls_dir_path) {
		this->add_log(::pilo::core::logging::level::error, "xls_conf_gen@0x%p xls dir path is null", this);
		return PERR_NULL_PATH;
	}
	else
		_xls_dir_path.set(xls_dir_path, xls_dir_path_base);

	if (nullptr == dest_server_config_dir_path)
		_dest_config_dir_path[xls_config_set::server].reset();
	else
		_dest_config_dir_path[xls_config_set::server].set(dest_server_config_dir_path, dest_server_config_dir_path_base);

	if (nullptr == dest_client_config_dir_path)
		_dest_config_dir_path[xls_config_set::client].reset();
	else
		_dest_config_dir_path[xls_config_set::client].set(dest_client_config_dir_path, dest_client_config_dir_path_base);

	if (nullptr == dest_server_source_dir_path)
		_dest_source_dir_path[xls_config_set::server].reset();
	else
		_dest_source_dir_path[xls_config_set::server].set(dest_server_source_dir_path, dest_server_source_dir_path_base);

	if (nullptr == dest_client_source_dir_path)
		_dest_source_dir_path[xls_config_set::client].reset();
	else
		_dest_source_dir_path[xls_config_set::client].set(dest_client_source_dir_path, dest_client_source_dir_path_base);

	return PILO_OK;
}

void pilo::core::config::xls_config_generator::clear()
{
	_xls_dir_path.reset();
	_dest_config_dir_path[xls_config_set::server].reset();
	_dest_config_dir_path[xls_config_set::client].reset();
	_dest_source_dir_path[xls_config_set::server].reset();
	_dest_source_dir_path[xls_config_set::client].reset();
	_logs.clear();
	_ccnames.clear();
	_scnames.clear();
	_ccfgs.clear();
	_scfgs.clear();
}

::pilo::err_t pilo::core::config::xls_config_generator::parse()
{
	this->add_log(::pilo::core::logging::level::info, "Start Parsing dir - [%s]", _xls_dir_path.fullpath());

	::pilo::err_t ret = ::pilo::core::io::path::dfs_travel_path(&_xls_dir_path, s_xls_file_iter_func, this, false, ::pilo::core::io::path::evt_node_visiting);
	if (ret != PILO_OK) {
		return ret;
	}

	std::stringstream ss;
	for (std::map<std::string, xls_config_set>::const_iterator cit = _config_set_map.cbegin(); cit != _config_set_map.cend(); cit ++) {
		ss.clear();
		ss.str("");
		ss << cit->first << "\n" << cit->second.to_string();
		this->add_log(::pilo::core::logging::level::debug, ss.str());
	}

	

	return PILO_OK;
}

::pilo::err_t pilo::core::config::xls_config_generator::generate_server_config()
{
	return _generate_config(xls_config_set::server, "Server");
}

::pilo::err_t pilo::core::config::xls_config_generator::generate_client_config()
{

	return _generate_config(xls_config_set::client, "Client");
}

::pilo::err_t pilo::core::config::xls_config_generator::generate_server_source()
{

	return PILO_OK;
}

::pilo::err_t pilo::core::config::xls_config_generator::generate_client_source()
{

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

void pilo::core::config::xls_config_generator::add_log(::pilo::core::logging::level level, const std::string& msg)
{
	char buf[PMI_XLS_GEN_ERR_BUFF_SIZE] = { 0 };
	::pilo::core::io::string_formated_output(buf, sizeof(buf), "XCG_0x%p => @[NA Pos] : ", this);

	std::string s = buf;
	s += msg;
	::pilo::core::logging::info_item ii((::pilo::u32_t)_logs.size(), ::pilo::core::logging::representative_type::text, level, 0, ::pilo::core::datetime::timestamp_micro_system(), s.c_str());
	_logs.push_back(ii);
}


