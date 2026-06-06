////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                //
//  .----------------.  .----------------.  .----------------.  .----------------.     PILO by    //
//  | .--------------. || .--------------. || .--------------. || .--------------. | 𝓓𝓪𝓶𝓷𝓮𝓭𝓒𝓪𝓽  //
//  | |   ______     | || |     _____    | || |   _____      | || |     ____     | |              //
//  | |  |_   __ \   | || |    |_   _|   | || |  |_   _|     | || |   .'    `.   | |  |\.-"-./|   //
//  | |    | |__) |  | || |      | |     | || |    | |       | || |  /  .--.  \  | |  \`     `/   //
//  | |    |  ___/   | || |      | |     | || |    | |   _   | || |  | |    | |  | |  |= ^Y^ =|   //
//  | |   _| |_      | || |     _| |_    | || |   _| |__/ |  | || |  \  `- - '/  | |  \__ ^ __/   //
//  | |  |_____|     | || |    |_____|   | || |  |________|  | || |   `.____.'   | |  /`=+o+=`\   //
//  | |              | || |              | || |              | || |              | | |         |  //
//  | '--------------' || '--------------' || '--------------' || '--------------' | | (     ) |  //
//  '----------------'  '----------------'  '----------------'  '----------------'   (,,)---(,,)  //
//                                                                                                //
////////////////////////////////////////////////////////////////////////////////////////////////////

#include	"xls_config.hpp"
#include	"../dp/xls_spread_sheet.hpp"
#include	<sstream>
#include	<algorithm>
#include	"../process/context.hpp"
#include    "../ml/json_tlv_driver.hpp"
#include	"../io/path.hpp"
#include	"../autogen/autogen.hpp"
#include	"../io/file.hpp"

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

	std::vector<::pilo::i32_t> idx_vec;
	for (size_t i = 0; i < vec.size(); i ++) {
		::pilo::i32_t fidx = cfg->find_filed_by_name(vec[i]);
		if (fidx < 0) {
			return err;
		}
		idx_vec.push_back(fidx);
	}

	cfg->_union_indices.push_back(std::move(idx_vec));
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
		_source_file_name = std::move(_source_file_name);
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
			ss << this->_fields[_union_indices[i][j]].name() << ',';
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

bool pilo::core::config::xls_config::check_uniqe(char* buff, ::pilo::i64_t buffsz) const
{
	for (size_t i = 0; i < _fields.size(); i++) {
		if (_fields.at(i).test_flag(xls_config_set::flag_unique)){
			if (_fields.at(i).test_flag(xls_config_set::flag_primary_key) 
				|| _fields.at(i).test_flag(xls_config_set::flag_primary_key_array)
				|| _fields.at(i).test_flag(xls_config_set::flag_index) 
				|| _fields.at(i).test_flag(xls_config_set::flag_nullable)) {
				::pilo::core::io::string_formated_output(buff, buffsz, "field_%llu (%s) mark as qunique, but conflict with p/P/i/n attributes those have been also assigned.", (::pilo::u64_t)i, _fields.at(i).name().c_str(), _fields.at(i).wrapper_type());
				return false;
			}

			if (! _fields.at(i).is_index_pk_unique()) {
				::pilo::core::io::string_formated_output(buff, buffsz, "field_%llu (%s) mark as unique, but has conflict wrapper type (%u) or value type (%u).", (::pilo::u64_t)i, _fields.at(i).name().c_str(), _fields.at(i).wrapper_type(), _fields.at(i).value_type());
				return false;
			}
			

		} else if (_fields.at(i).test_flag(xls_config_set::flag_primary_key)) {
			if (_fields.at(i).test_flag(xls_config_set::flag_primary_key_array)
				|| _fields.at(i).test_flag(xls_config_set::flag_index)
				|| _fields.at(i).test_flag(xls_config_set::flag_nullable)) {
				::pilo::core::io::string_formated_output(buff, buffsz, "field_%llu (%s) mark as PK, but conflict with u/P/i/n attributes those have been also assigned.", (::pilo::u64_t)i, _fields.at(i).name().c_str(), _fields.at(i).wrapper_type());
				return false;
			}

			if (!_fields.at(i).is_index_pk_unique()) {
				::pilo::core::io::string_formated_output(buff, buffsz, "field_%llu (%s) mark as PK, but has conflict wrapper type (%u) or value type (%u).", (::pilo::u64_t)i, _fields.at(i).name().c_str(), _fields.at(i).wrapper_type(), _fields.at(i).value_type());
				return false;
			}
		} else if (_fields.at(i).test_flag(xls_config_set::flag_index)) {
			if (_fields.at(i).test_flag(xls_config_set::flag_primary_key_array)
				|| _fields.at(i).test_flag(xls_config_set::flag_primary_key)) {
				::pilo::core::io::string_formated_output(buff, buffsz, "field_%llu (%s) mark as Index, but conflict with u/P/P/ attributes those have been also assigned.", (::pilo::u64_t)i, _fields.at(i).name().c_str(), _fields.at(i).wrapper_type());
				return false;
			}

			if (!_fields.at(i).is_index_pk_unique()) {
				::pilo::core::io::string_formated_output(buff, buffsz, "field_%llu (%s) mark as Index, but has conflict wrapper type (%u) or value type (%u).", (::pilo::u64_t)i, _fields.at(i).name().c_str(), _fields.at(i).wrapper_type(), _fields.at(i).value_type());
				return false;
			}
		} else if (_fields.at(i).test_flag(xls_config_set::flag_primary_key_array)) {
			if ( _fields.at(i).test_flag(xls_config_set::flag_index)
				|| _fields.at(i).test_flag(xls_config_set::flag_nullable)) {
				::pilo::core::io::string_formated_output(buff, buffsz, "field_%llu (%s) mark as PK_arr, but conflict with u/p/i/n attributes those have been also assigned.", (::pilo::u64_t)i, _fields.at(i).name().c_str(), _fields.at(i).wrapper_type());
				return false;
			}

			if (!_fields.at(i).is_pk_arr()) {
				::pilo::core::io::string_formated_output(buff, buffsz, "field_%llu (%s) mark as Index, but has conflict wrapper type (%u) or value type (%u).", (::pilo::u64_t)i, _fields.at(i).name().c_str(), _fields.at(i).wrapper_type(), _fields.at(i).value_type());
				return false;
			}
		}

		if (_fields.at(i).test_flag(xls_config_set::flag_unique) || _fields.at(i).test_flag(xls_config_set::flag_primary_key)) {
			if (_fields.at(i).value_type() == ::pilo::core::rtti::wired_type::value_type_str) {
				if (!_check_uniqe_str(_fields.at(i), buff, buffsz)) {
					return false;
				}
			} else if (_fields.at(i).is_value_type_int()) {
				if (!_check_uniqe_int(_fields.at(i), buff, buffsz)) {
					return false;
				}
			} else if (_fields.at(i).is_value_type_uint()) {
				if (!_check_uniqe_uint(_fields.at(i), buff, buffsz)) {
					return false;
				}
			} 

		} else if (_fields.at(i).test_flag(xls_config_set::flag_primary_key_array)) {
			if (_fields.at(i).is_value_type_int()) {
				if (!_check_pk_array_int(_fields.at(i), buff, buffsz)) {
					return false;
				}
			}
			else if (_fields.at(i).is_value_type_uint()) {
				if (!_check_pk_array_uint(_fields.at(i), buff, buffsz)) {
					return false;
				}
			}
		}
	}

	



	return true;
}

bool pilo::core::config::xls_config::_check_uniqe_str(const xls_config_field& fld_cref, char* buff, ::pilo::i64_t buffsz) const
{
	::pilo::err_t err = PILO_OK;
	::pilo::i32_t exist_line_no = -1;
	::pilo::duplicate_map_checker<std::string, ::pilo::i32_t> checker;

	for (::pilo::i32_t j = 0; j < _data->size(); j++) {

		::pilo::tlv* record_tlv_ptr = _data->get<::pilo::tlv*>(j, &err);
		if (record_tlv_ptr == nullptr || err != PILO_OK) {
			::pilo::core::io::string_formated_output(buff, buffsz, "Get Record_%d form root TLV data Failed. (%d)", j,  err);
			return false;
		}

		::pilo::tlv* ret_tlv = nullptr;
		err = record_tlv_ptr->get<std::string, ::pilo::tlv*>(fld_cref.name(), ret_tlv);
		if (record_tlv_ptr == nullptr || err != PILO_OK) {
			::pilo::core::io::string_formated_output(buff, buffsz, "Get Field (%s) from Record_%d Failed. (%d)", fld_cref.name().c_str(), j, err);
			return false;
		}

		const std::string* strp = ret_tlv->string_ptr();
		if (strp == nullptr) {
			::pilo::core::io::string_formated_output(buff, buffsz, "Get string value from field (%s) Failed. (%d)", fld_cref.name().c_str(), j);
			return false;
		}
		
		if (! checker.check(*strp, j, exist_line_no)) {
			::pilo::core::io::string_formated_output(buff, buffsz, "Duplciated value (%s) of field (%s) Found at record %d and %d", strp->c_str(), fld_cref.name().c_str(), j, exist_line_no);
			return false;
		}
	}
	return true;
}

bool pilo::core::config::xls_config::_check_uniqe_int(const xls_config_field& fld_cref, char* buff, ::pilo::i64_t buffsz) const
{
	::pilo::err_t err = PILO_OK;
	::pilo::i32_t exist_line_no = -1;
	::pilo::duplicate_map_checker<::pilo::i64_t, ::pilo::i32_t> checker;

	for (::pilo::i32_t j = 0; j < _data->size(); j++) {

		::pilo::tlv* record_tlv_ptr = _data->get<::pilo::tlv*>(j, &err);
		if (record_tlv_ptr == nullptr || err != PILO_OK) {
			::pilo::core::io::string_formated_output(buff, buffsz, "Get Record_%d form root TLV data Failed. (%d)", j, err);
			return false;
		}

		::pilo::tlv* ret_tlv = nullptr;
		err = record_tlv_ptr->get<std::string, ::pilo::tlv*>(fld_cref.name(), ret_tlv);
		if (record_tlv_ptr == nullptr || err != PILO_OK) {
			::pilo::core::io::string_formated_output(buff, buffsz, "Get Field (%s) from Record_%d Failed. (%d)", fld_cref.name().c_str(), j, err);
			return false;
		}

		::pilo::i64_t iv = ret_tlv->as_i64(&err);
		if (err != PILO_OK) {
			::pilo::core::io::string_formated_output(buff, buffsz, "Get int value from field (%s) Failed. (%d)", fld_cref.name().c_str(), j);
			return false;
		}

		if (!checker.check(iv, j, exist_line_no)) {
			::pilo::core::io::string_formated_output(buff, buffsz, "Duplciated value (%lld) of field (%s) Found at record %d and %d", iv, fld_cref.name().c_str(), j, exist_line_no);
			return false;
		}
	}
	return true;
}

bool pilo::core::config::xls_config::_check_uniqe_uint(const xls_config_field& fld_cref, char* buff, ::pilo::i64_t buffsz) const
{
	::pilo::err_t err = PILO_OK;
	::pilo::i32_t exist_line_no = -1;
	::pilo::duplicate_map_checker<::pilo::u64_t, ::pilo::i32_t> checker;

	for (::pilo::i32_t j = 0; j < _data->size(); j++) {

		::pilo::tlv* record_tlv_ptr = _data->get<::pilo::tlv*>(j, &err);
		if (record_tlv_ptr == nullptr || err != PILO_OK) {
			::pilo::core::io::string_formated_output(buff, buffsz, "Get Record_%d form root TLV data Failed. (%d)", j, err);
			return false;
		}

		::pilo::tlv* ret_tlv = nullptr;
		err = record_tlv_ptr->get<std::string, ::pilo::tlv*>(fld_cref.name(), ret_tlv);
		if (record_tlv_ptr == nullptr || err != PILO_OK) {
			::pilo::core::io::string_formated_output(buff, buffsz, "Get Field (%s) from Record_%d Failed. (%d)", fld_cref.name().c_str(), j, err);
			return false;
		}

		::pilo::u64_t iv = ret_tlv->as_u64(&err);
		if (err != PILO_OK) {
			::pilo::core::io::string_formated_output(buff, buffsz, "Get int value from field (%s) Failed. (%d)", fld_cref.name().c_str(), j);
			return false;
		}

		if (!checker.check(iv, j, exist_line_no)) {
			::pilo::core::io::string_formated_output(buff, buffsz, "Duplciated value (%lld) of field (%s) Found at record %d and %d", iv, fld_cref.name().c_str(), j, exist_line_no);
			return false;
		}
	}
	return true;
}

bool pilo::core::config::xls_config::_check_pk_array_int(const xls_config_field& fld_cref, char* buff, ::pilo::i64_t buffsz) const
{
	::std::map<::pilo::i64_t, ::pilo::i32_t>	exsitence_map;
	::pilo::err_t err = PILO_OK;
	for (::pilo::i32_t j = 0; j < _data->size(); j++) {

		::pilo::tlv* record_tlv_ptr = _data->get<::pilo::tlv*>(j, &err);
		if (record_tlv_ptr == nullptr || err != PILO_OK) {
			::pilo::core::io::string_formated_output(buff, buffsz, "Get Record_%d form root TLV data Failed. (%d)", j, err);
			return false;
		}

		::pilo::tlv* ret_tlv = nullptr;
		err = record_tlv_ptr->get<std::string, ::pilo::tlv*>(fld_cref.name(), ret_tlv);
		if (record_tlv_ptr == nullptr || err != PILO_OK) {
			::pilo::core::io::string_formated_output(buff, buffsz, "Get Field (%s) from Record_%d Failed. (%d)", fld_cref.name().c_str(), j, err);
			return false;
		}

		::pilo::i64_t iv = ret_tlv->as_i64(&err);
		if (err != PILO_OK) {
			::pilo::core::io::string_formated_output(buff, buffsz, "Get int value from field (%s) Failed. (%d)", fld_cref.name().c_str(), j);
			return false;
		}

		::std::map<::pilo::i64_t, ::pilo::i32_t>::const_iterator cit = exsitence_map.find(iv);
		if (cit != exsitence_map.cend() ) {
			::pilo::core::io::string_formated_output(buff, buffsz, "Duplciated value (%lld) of field (%s) Found at record %d and %d", iv, fld_cref.name().c_str(), j, cit->second);
			return false;
		}

		exsitence_map.insert(std::pair<::pilo::i64_t, ::pilo::i32_t>(iv, j));		
	}

	if (exsitence_map.size() != (size_t) _data->size() ) {
		::pilo::core::io::string_formated_output(buff, buffsz, "Found duplicated value exists for field (%s), may be insert failed.",  fld_cref.name().c_str());
		return false;
	}

	if (exsitence_map.begin()->first != 0) {
		::pilo::core::io::string_formated_output(buff, buffsz, "Field (%s) NOT begin from ZERO, rules voilation.", fld_cref.name().c_str());
		return false;
	}

	if ((int) exsitence_map.rbegin()->first !=  _data->size() - 1 ) {
		::pilo::core::io::string_formated_output(buff, buffsz, "Field (%s) NOT end at %d, rules voilation.", fld_cref.name().c_str(), (int)_data->size() - 1);
		return false;
	}


	return true;

}

bool pilo::core::config::xls_config::_check_pk_array_uint(const xls_config_field& fld_cref, char* buff, ::pilo::i64_t buffsz) const
{
	::std::map<::pilo::u64_t, ::pilo::i32_t>	exsitence_map;
	::pilo::err_t err = PILO_OK;
	for (::pilo::i32_t j = 0; j < _data->size(); j++) {

		::pilo::tlv* record_tlv_ptr = _data->get<::pilo::tlv*>(j, &err);
		if (record_tlv_ptr == nullptr || err != PILO_OK) {
			::pilo::core::io::string_formated_output(buff, buffsz, "Get Record_%d form root TLV data Failed. (%d)", j, err);
			return false;
		}

		::pilo::tlv* ret_tlv = nullptr;
		err = record_tlv_ptr->get<std::string, ::pilo::tlv*>(fld_cref.name(), ret_tlv);
		if (record_tlv_ptr == nullptr || err != PILO_OK) {
			::pilo::core::io::string_formated_output(buff, buffsz, "Get Field (%s) from Record_%d Failed. (%d)", fld_cref.name().c_str(), j, err);
			return false;
		}

		::pilo::u64_t uv = ret_tlv->as_u64(&err);
		if (err != PILO_OK) {
			::pilo::core::io::string_formated_output(buff, buffsz, "Get int value from field (%s) Failed. (%d)", fld_cref.name().c_str(), j);
			return false;
		}

		::std::map<::pilo::u64_t, ::pilo::i32_t>::const_iterator cit = exsitence_map.find(uv);
		if (cit != exsitence_map.cend()) {
			::pilo::core::io::string_formated_output(buff, buffsz, "Duplciated value (%llu) of field (%s) Found at record %d and %d", uv, fld_cref.name().c_str(), j, cit->second);
			return false;
		}

		exsitence_map.insert(std::pair<::pilo::u64_t, ::pilo::i32_t>(uv, j));
	}

	if (exsitence_map.size() != (size_t)_data->size()) {
		::pilo::core::io::string_formated_output(buff, buffsz, "Found duplicated value exists for field (%s), may be insert failed.", fld_cref.name().c_str());
		return false;
	}

	if (exsitence_map.begin()->first != 0) {
		::pilo::core::io::string_formated_output(buff, buffsz, "Field (%s) NOT begin from ZERO, rules voilation.", fld_cref.name().c_str());
		return false;
	}

	if ((::pilo::i64_t)exsitence_map.begin()->first != (::pilo::i64_t) _data->size() - 1) {
		::pilo::core::io::string_formated_output(buff, buffsz, "Field (%s) NOT end at %d, rules voilation.", fld_cref.name().c_str(), (int)_data->size() - 1);
		return false;
	}


	return true;
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
		PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Cell@%u:%u: Low Count (%lld) Split field spec Failed (%s) in %s.%s", row, col, rcnt, field_str.c_str(), xlsfullfilepath, wsnamecstr);
		return ::pilo::mk_perr(PERR_INC_DATA);
	}


	//parse pri
	if (raw_fields[0].is_empty()) {
		f._pri = cfg_set._configs[which].find_lowest_pri_field();
	}
	else {
		err = ::pilo::core::string::string_to_number(f._pri, raw_fields[0].ptr, raw_fields[0].length);
		if (err != PILO_OK) {
			PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Cell@%u:%u: Pri to number Failed (%s) in %s.%s", row, col, raw_fields[0], xlsfullfilepath, wsnamecstr);
			return ::pilo::mk_perr(PERR_INC_DATA);
		}
	}

	//parse name
	if (raw_fields[1].is_empty()) {
		PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Cell@%u:%u: Name is Empty in %s.%s", row, col, xlsfullfilepath, wsnamecstr);
		return ::pilo::mk_perr(PERR_INC_DATA);
	}
	else {
		f._name.assign(raw_fields[1].ptr, raw_fields[1].length);
	}
	::pilo::i32_t existing_fld_pos = cfg_set._configs[which].find_existing_field_idx_by_name(f.name());
	if (existing_fld_pos >= 0) {
		PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Cell@%u:%u: Name (%s) is existing at field_%d in %s.%s", row, col, f._name.c_str(), xlsfullfilepath, wsnamecstr);
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
		PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Cell@%u:%u: Type (%s) parse Failed. in %s.%s", row, col, type_tmp_str.c_str(), xlsfullfilepath, wsnamecstr);
		return ::pilo::mk_perr(PERR_INC_DATA);
	}
	if (wt == ::pilo::core::rtti::wired_type::wrapper_single) {
		if (vt == ::pilo::core::rtti::wired_type::value_type_na) {
			PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Cell@%u:%u: Type (%s) parse Failed. Got NA type for single type.  in %s.%s", row, col, type_tmp_str.c_str(), xlsfullfilepath, wsnamecstr);
			return ::pilo::mk_perr(PERR_INC_DATA);
		}
	}
	else if (wt == ::pilo::core::rtti::wired_type::wrapper_array) {
		if (vt == ::pilo::core::rtti::wired_type::value_type_na) {
			PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Cell@%u:%u: Type (%s) parse Failed. Got NA type for array type. in %s.%s", row, col, type_tmp_str.c_str(), xlsfullfilepath, wsnamecstr);
			return ::pilo::mk_perr(PERR_INC_DATA);
		}
	}
	else if (wt == ::pilo::core::rtti::wired_type::wrapper_dict) {
		if (kt == ::pilo::core::rtti::wired_type::key_type_na) {
			PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Cell@%u:%u: Type (%s) parse Failed. Got NA key type for hash type. in %s.%s", row, col, type_tmp_str.c_str(), xlsfullfilepath, wsnamecstr);
			return ::pilo::mk_perr(PERR_INC_DATA);
		}
		if (vt == ::pilo::core::rtti::wired_type::value_type_na) {
			PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Cell@%u:%u: Type (%s) parse Failed. Got NA val type for hash type. in %s.%s", row, col, type_tmp_str.c_str(), xlsfullfilepath, wsnamecstr);
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
				PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Cell@%u:%u: Attr (%s) parse Failed. Got Invalid marker (%c) for field %s. in %s.%s", row, col, attr_str.c_str(), attr_str.at(k), f.name().c_str(), xlsfullfilepath, wsnamecstr);
				return ::pilo::mk_perr(PERR_INC_DATA);
			}
		}
	}
	if (f._flags.test_value(xls_config_set::flag_primary_key) && f._flags.test_value(xls_config_set::flag_primary_key_array)) {
		PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Cell@%u:%u: Attr (%s) parse Failed. Got pk and pk_arr both assigned. in %s.%s", row, col, attr_str.c_str(), xlsfullfilepath, wsnamecstr);
		return ::pilo::mk_perr(PERR_INC_DATA);
	}

	//default value
	if (rcnt > 4 && raw_fields[4].length > 0) {
		f._default_value_str.assign(raw_fields[4].ptr, raw_fields[4].length);
		if (f._flags.test_value(xls_config_set::flag_nullable)) {
			PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Cell@%u:%u: Default value (%s) Found but attr has no nullable assigned. in %s.%s", row, col, f._default_value_str.c_str(), xlsfullfilepath, wsnamecstr);
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
			PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Cell@%u:%u: Alloc tlv Failed. in %s.%s", row, colno, xlsfullfilepath, wsnamecstr);
			return mk_err(PERR_INSUF_HEAP);
		}
		err = val_ptr->set_types(cfg_ref._fields.at(i).wrapper_type(), cfg_ref._fields.at(i).key_type(), cfg_ref._fields.at(i).value_type());
		if (PILO_OK != err) {
			PILO_CONTEXT->deallocate_tlv(val_ptr);
			PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Cell@%u:%u: Set type for tlv Failed (%s). in %s.%s", row, colno, ::pilo::str_err(err).c_str(), xlsfullfilepath, wsnamecstr);
			return err;
		}

		err = wsp->value(val_ptr, row, colno, cfg_ref._fields.at(i).test_flag(xls_config_set::flag_nullable), cfg_ref._fields.at(i).default_value(), errmsgbuff, sizeof(errmsgbuff));
		if (PILO_OK != err) {
			PILO_CONTEXT->deallocate_tlv(val_ptr);
			PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Cell@%u:%u: Compse tlv from cell Failed (%s).in %s.%s", row, colno, errmsgbuff, xlsfullfilepath, wsnamecstr);
			return err;
		}

		err = record->insert<std::string, ::pilo::tlv*>(cfg_ref._fields.at(i).name(), val_ptr, false);
		if (PILO_OK != err) {
			PILO_CONTEXT->deallocate_tlv(val_ptr);
			PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Cell@%u:%u: Insert tlv to record Failed (%s). in %s.%s", row, colno, ::pilo::str_err(err).c_str(), xlsfullfilepath, wsnamecstr);
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
		PMF_APPEND_TEXT_LOG_DFL((pgen->log_set()), ::pilo::core::logging::level::error, "File - [%s] Ignored", src_path->fullpath());
		return PILO_OK;
	}

	if ((::pilo::core::string::i_compare(src_path->extname(), 0, "xlsx", 0, -1))
		&& (::pilo::core::string::i_compare(src_path->extname(), 0, "xls", 0, -1))) {
		PMF_APPEND_TEXT_LOG_DFL((pgen->log_set()), ::pilo::core::logging::level::error, "File - [%s] Ignored", src_path->fullpath());
		return PILO_OK;
	}

	::pilo::pathlen_t rlen = 0;
	if (src_path->basename(rlen)[0] == '~') {
		PMF_APPEND_TEXT_LOG_DFL((pgen->log_set()), ::pilo::core::logging::level::error, "File - [%s] Ignored", src_path->fullpath());
		return PILO_OK;
	}

	if (fsnt != ::pilo::core::io::path::fs_node_type_file) {
		PMF_APPEND_TEXT_LOG_DFL((pgen->log_set()), ::pilo::core::logging::level::error, "File - [%s] Ignored", src_path->fullpath());
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
	PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::info, "Parsing File - [%s]", xls_fullpath);

	::pilo::core::dp::xls_spread_document doc;
	::pilo::err_t eret = doc.open(xls_fullpath, ::pilo::core::io::creation_mode::open_existing, ::pilo::predefined_pilo_path::count);
	if (eret != PILO_OK) {
		PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Open File Failed - [%s].", xls_fullpath);
		return eret;
	}

	std::vector<std::string> name_lists;
	eret = doc.get_all_worksheet_names(name_lists);
	if (eret != PILO_OK) {
		PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Obtain worksheets name list of [%s] Failed.", xls_fullpath);
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
		PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::warn, "Var of %s not found in %s.%s, fallback to %s (%s) as its value", t1, file, wsname, t2, b.c_str());
		a = b;
		return true;
	} else if (b.empty()) {
		PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::warn, "var of %s not found in %s.%s, fallback to %s (%s) as its value", t2, file, wsname, t1, b.c_str());
		b = a;
		return true;
	}
	PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::warn, "value for both %s & %s of %s.%s not found", t1, t2, file, wsname);
	return false;
}

bool pilo::core::config::xls_config_generator::_check_duplicate_vars_in_header(const std::string& ccname, const std::string& scname, const std::string& ccfg, const std::string& scfg, const char* file, const char* wsname)
{
	std::map<std::string, std::string>::const_iterator cit = _ccnames.find(ccname);
	if (cit != _ccnames.cend()) {
		PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Duplicate CCNAME (%s) in %s.%s and %s", ccname.c_str(), file, wsname, cit->second.c_str());
		return false;
	}

	cit = _scnames.find(scname);
	if (cit != _scnames.cend()) {
		PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Duplicate SCNAME (%s) in %s.%s and %s", scname.c_str(), file, wsname, cit->second.c_str());
		return false;
	}

	cit = _ccfgs.find(ccfg);
	if (cit != _ccfgs.cend()) {
		PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Duplicate CCFG (%s) in %s.%s and %s", ccfg.c_str(), file, wsname, cit->second.c_str());
		return false;
	}

	cit = _scfgs.find(scfg);
	if (cit != _scfgs.cend()) {
		PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Duplicate SCFG (%s) in %s.%s and %s", scfg.c_str(), file, wsname, cit->second.c_str());
		return false;
	}
	
	return true;
}

::pilo::err_t pilo::core::config::xls_config_generator::_generate_config(int which)
{
	::pilo::core::io::path dst_path;
	::pilo::err_t err = PILO_OK;
	std::map<std::string, xls_config_set>::const_iterator cit = _config_set_map.cbegin();	

	err = _dest_config_dir_path[which].create(::pilo::core::io::path::fs_node_type_dir, false);	
	if (err != PILO_OK) {
		PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "%s Config File Dest Path [%s] create Failed.", xls_config_set::config_type_tags[which], _dest_config_dir_path[which].fullpath());
		return mk_err(err);
	}

	for (; cit != _config_set_map.cend(); cit++) {
		std::string fname;
		fname = ::pilo::core::string::rfind_substring(cit->first.c_str(), PMS_PATH_SEP_S_A, -1);
		if (fname[0] == PMI_PATH_SEP) {
			fname = fname.substr(1);
		}

		if (cit->second._configs[which].config_file_name().empty()) {
			PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "%s Config File of (%s) Skipped, because of absent config name.", xls_config_set::config_type_tags[which], fname.c_str());
			continue;
		}

		dst_path = _dest_config_dir_path[which];
		err = dst_path.append(cit->second._configs[which].config_file_name().c_str());
		if (err != PILO_OK) {
			PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "%s Config File generated failed. dest path compose Failed. append %s -> %s", xls_config_set::config_type_tags[which], cit->second._configs[xls_config_set::server].config_file_name().c_str(), dst_path.fullpath());
			return mk_err(err);
		}

		::pilo::core::ml::json_tlv_driver jdrv(cit->second._configs[which]._data);
		err = jdrv.save(&dst_path);
		if (err != PILO_OK) {
			PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "%s Config File generated failed. [%s] Serialize to Json Failedd %d ", xls_config_set::config_type_tags[which], cit->second._configs[xls_config_set::server].config_file_name().c_str(), err);
			return mk_err(err);
		}

		PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::info, "%s Config File [%s] has been created successfully.", xls_config_set::config_type_tags[which], dst_path.fullpath());

	}

	PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::info, "All %s Config File generation Done", xls_config_set::config_type_tags[which]);

	return PILO_OK;
}

::pilo::err_t pilo::core::config::xls_config_generator::_generate_source(int which, ::pilo::core::autogen::lang_type ltype)
{
	::pilo::core::io::path dst_path;
	::pilo::err_t err = PILO_OK;
	std::map<std::string, xls_config_set>::const_iterator cit = _config_set_map.cbegin();

	err = _dest_source_dir_path[which].create(::pilo::core::io::path::fs_node_type_dir, false);
	if (err != PILO_OK) {
		PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "%s Source File Dest Path [%s] create Failed.", xls_config_set::config_type_tags[which], _dest_config_dir_path[which].fullpath());
		return mk_err(err);
	}

	for (; cit != _config_set_map.cend(); cit++) {
		std::string fname;
		fname = ::pilo::core::string::rfind_substring(cit->first.c_str(), PMS_PATH_SEP_S_A, -1);
		if (fname[0] == PMI_PATH_SEP) {
			fname = fname.substr(1);
		}

		if (cit->second._configs[which].source_file_name().empty()) {
			PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "%s Source File of (%s) Skipped, because of absent config name.", xls_config_set::config_type_tags[which], fname.c_str());
			continue;
		}

		const xls_config_set& conf_set_ref = cit->second;

		if ((err = _generate_one_source(fname, which,conf_set_ref, ltype)) != PILO_OK) {
			return mk_err(err);
		}

	}

	PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::info, "All %s Source File generation Done", xls_config_set::config_type_tags[which]);

	return PILO_OK;
}

::pilo::err_t pilo::core::config::xls_config_generator::_generate_one_source(const std::string& cfg_pos, int which, const xls_config_set& conf_set_ref, ::pilo::core::autogen::lang_type ltype)
{
	const xls_config& conf_ref = conf_set_ref._configs[which];	
	::pilo::err_t err = PILO_OK;

	if (ltype == ::pilo::core::autogen::lang_type::na) {
		PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Invalid lang type (%d) for source %s from %s.", (int)ltype, conf_ref.cls_name().c_str(), cfg_pos.c_str());
	}	
	
	if (ltype == ::pilo::core::autogen::lang_type::cpp) {
		err = _generate_one_source_cpp(cfg_pos, which, conf_set_ref);
	}

	if (err != PILO_OK) {
		return err;
	}	
	
	return PILO_OK;
}

::pilo::err_t pilo::core::config::xls_config_generator::_generate_one_source_cpp(const std::string& cfg_pos, int which, const xls_config_set& conf_set_ref)
{
	const xls_config& conf_ref = conf_set_ref._configs[which];
	
	std::string hppname = conf_ref.cls_name() + ".hpp";
	std::string item_klass_name = conf_ref.cls_name() + "_item";
	const char* conf_target_type_str = xls_config_set::config_type_tags[which];
	std::string subpath = conf_ref.ns();
	replace(subpath.begin(), subpath.end(), '.', PMI_PATH_SEP);
	std::string hdr_filepath;
	std::string src_filepath;

	::pilo::core::autogen::meta_srcfile msf(0);
	msf.add_import(PMI_OS_UNSUPPORT, true, true, "cstdio");
	msf.add_import(PMI_OS_UNSUPPORT, true, true, "cstdlib");
	msf.add_import(PMI_OS_UNSUPPORT, true, true, "string");
	msf.add_import(PMI_OS_UNSUPPORT, true, true, "cstdint");
	msf.add_import(PMI_OS_UNSUPPORT, true, true, "vector");
	msf.add_import(PMI_OS_UNSUPPORT, true, true, "map");
	msf.add_import(PMI_OS_UNSUPPORT, true, true, "set");
	msf.add_import(PMI_OS_UNSUPPORT, false, false, hppname);

	msf.add_empty_lines(1);

	::pilo::core::autogen::meta_ns* nsp = msf.get_or_create_ns_node(conf_ref.ns());
	if (nsp == nullptr) {
		PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Creating ns (%s) failed for (%s) from %s. ", conf_ref.ns().c_str(), conf_ref.cls_name().c_str(), cfg_pos.c_str());
		return ::pilo::mk_perr(PERR_NULL_PTR);
	}

	//add item klass 
	::pilo::core::autogen::meta_klass* item_klass = nsp->add_klass_node(0, item_klass_name, "", 0);
	if (item_klass == nullptr) {
		PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Creating Item Klass (%s) failed for (%s) from %s. ", item_klass_name.c_str(), conf_ref.cls_name().c_str(), cfg_pos.c_str());
		return ::pilo::mk_perr(PERR_NULL_PTR);
	}
	std::string type_str;
	int mod_tmp = ::pilo::core::autogen::mod_private;
	::pilo::err_t err = PILO_OK;

	for (size_t i = 0; i < conf_ref.field_count(); i ++) {
		const xls_config_field* fldp = conf_ref.find_filed_by_pri_index(i);
		if (fldp == nullptr) {
			return ::pilo::mk_perr(PERR_NULL_PTR);
		}

		err = item_klass->add_wired_member_variable(fldp->_wired_type, mod_tmp, ::pilo::core::autogen::getter_rtype, fldp->name(), fldp->default_value(), "", "");
		if (err != PILO_OK) {
			PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Invalid Value type (%u) for Field (%s.%s) from %s. ", fldp->value_type(), item_klass_name.c_str(), fldp->name().c_str(), cfg_pos.c_str());
			return err;
		}	

	}
	::pilo::core::autogen::meta_function* funcp = item_klass->add_constructor(::pilo::core::autogen::mod_public | ::pilo::core::autogen::mod_autofill, {});
	if (funcp == nullptr) {
		PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Constructor of Klass (%s) created failed from %s. ", item_klass->name().c_str(), cfg_pos.c_str());
		return ::pilo::mk_perr(PERR_NULL_PTR);;
	}
	for (size_t i = 0; i < conf_ref.field_count(); i++) {
		const xls_config_field* fldp = conf_ref.find_filed_by_pri_index(i);
		if (fldp == nullptr) {
			return ::pilo::mk_perr(PERR_NULL_PTR);
		}

		err = funcp->add_wired_type_param(::pilo::core::autogen::mod_map_to_member | ::pilo::core::autogen::mod_private, fldp->name(), fldp->_wired_type, fldp->default_value(), "", "");
		if (err != PILO_OK) {
			PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Add param for paramlized Constructor (%s) failed (%s) from %s. ", item_klass_name.c_str(), fldp->name().c_str(), cfg_pos.c_str());
			return err;
		}
	}

	mod_tmp = ::pilo::core::autogen::mod_public | ::pilo::core::autogen::mod_autofill;
	funcp = item_klass->add_constructor(mod_tmp, { });
	if (funcp == nullptr) {
		PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Constructor of Klass (%s) created failed from %s. ", item_klass->name().c_str(), cfg_pos.c_str());
		return ::pilo::mk_perr(PERR_NULL_PTR);;
	}

	funcp = item_klass->add_desstructor(::pilo::core::autogen::mod_public);
	if (funcp == nullptr) {
		PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Destructor of Klass (%s) created failed from %s. ", item_klass->name().c_str(), cfg_pos.c_str());
		return ::pilo::mk_perr(PERR_NULL_PTR);;
	}

	funcp = item_klass->add_copy_constructor(::pilo::core::autogen::mod_public | ::pilo::core::autogen::mod_autofill);
	if (funcp == nullptr) {
		PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Copy Constructor of Klass (%s) created failed from %s. ", item_klass->name().c_str(), cfg_pos.c_str());
		return ::pilo::mk_perr(PERR_NULL_PTR);;
	}

	funcp = item_klass->add_copy_operator(::pilo::core::autogen::mod_public | ::pilo::core::autogen::mod_autofill);
	if (funcp == nullptr) {
		PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Copy Operator of Klass (%s) created failed from %s. ", item_klass->name().c_str(), cfg_pos.c_str());
		return ::pilo::mk_perr(PERR_NULL_PTR);;
	}

	funcp = item_klass->add_move_constructor(::pilo::core::autogen::mod_public | ::pilo::core::autogen::mod_autofill);
	if (funcp == nullptr) {
		PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Move Constructor of Klass (%s) created failed from %s. ", item_klass->name().c_str(), cfg_pos.c_str());
		return ::pilo::mk_perr(PERR_NULL_PTR);;
	}

	funcp = item_klass->add_move_operator(::pilo::core::autogen::mod_public | ::pilo::core::autogen::mod_autofill);
	if (funcp == nullptr) {
		PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Move Operator of Klass (%s) created failed from %s. ", item_klass->name().c_str(), cfg_pos.c_str());
		return ::pilo::mk_perr(PERR_NULL_PTR);;
	}




	std::stringstream ss;
	msf.append_to_stringstream_cpp(ss, ::pilo::core::autogen::oflag_dec, "");
	::pilo::core::io::path dstpath = _dest_source_dir_path[which];
	dstpath.append(subpath.c_str());
	err = _save_souce_file(ss, &dstpath, conf_ref.cls_name(), ".hpp");
	if (err != PILO_OK)
		return err;	
	printf("\n-------------------------------\n");
	printf("%s", ss.str().c_str());
	printf("\n-------------------------------\n");

	ss.str("");
	msf.append_to_stringstream_cpp(ss, 0, "");
	printf("\n-------------------------------\n");
	printf("%s", ss.str().c_str());
	printf("\n-------------------------------\n");

	err = _save_souce_file(ss, &dstpath, conf_ref.cls_name(), ".cpp");
	if (err != PILO_OK)
		return err;

	PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::info, "%s Source File for Class (%s) (%s) generated successfully from %s. ", conf_target_type_str, conf_ref.cls_name().c_str(), conf_ref.source_file_name().c_str(), cfg_pos.c_str());


	

	


	return PILO_OK;
}

::pilo::err_t pilo::core::config::xls_config_generator::_save_souce_file(std::stringstream& ss, const::pilo::core::io::path* dir, const std::string filenamestr, const std::string& ext)
{
	std::string fullpath = dir->fullpath();
	fullpath += PMI_PATH_SEP + filenamestr + ext;
	::pilo::core::io::file f;
	::pilo::err_t err  = f.set_path(fullpath);
	if (err != PILO_OK) {
		PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Source File generated failed. Set path (%s) to file Failed", fullpath.c_str());
		return err;
	}
	err = f.open(::pilo::core::io::creation_mode::create_always, ::pilo::core::io::access_permission::write, ::pilo::core::io::dev_open_flags::none);
	if (err != PILO_OK) {
		PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Source File generated failed. Open file (%s) Failed",  fullpath.c_str());
		return mk_err(err);
	}

	std::string strbuf = ss.str();
	::pilo::i64_t bs_wrote = 0;
	err = f.write(strbuf.c_str(), (::pilo::i64_t)strbuf.size(), &bs_wrote);
	if (err != PILO_OK || bs_wrote != (::pilo::i64_t)strbuf.size()) {
		PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Source File generated failed. Write file (%s) Failed bs=(%lld)", fullpath.c_str(), bs_wrote);
		return mk_err(err);
	}

	f.close();

	return ::pilo::err_t();
}

::pilo::err_t pilo::core::config::xls_config_generator::_parse_worksheet(::pilo::core::dp::xls_spread_document& doc, const char* xls_fullpathname, ::pilo::u32_t ws_idx)
{	
	::pilo::core::dp::xls_spread_sheet ws = doc.worksheet_by_index(ws_idx);
	std::string wsname = ws.name();	

	PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::info, "Parsing Sheet [%s] in File - [%s]", wsname.c_str(), xls_fullpathname);

	std::string tmp_cell_str, tmp_varname_cell;
	::pilo::err_t err = PILO_OK;
	::pilo::u32_t rcnt = ws.row_count();
	bool c_header_done = false;
	bool s_header_done = false;
	char errbuff[PMI_XLS_GEN_ERR_BUFF_SIZE] = {0};
	std::string strkvpair[2];
	std::string desc, ccname, scname, ccfg, scfg, cns, sns, cui, sui, ssrc, csrc;

	std::string filepathname_n_ws = xls_fullpathname;
	filepathname_n_ws += '.';
	filepathname_n_ws += wsname;

	if (_config_set_map.find(filepathname_n_ws) != _config_set_map.cend()) {
		PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Duplicate xls file and sheet exists. %s.%s", xls_fullpathname, wsname.c_str());
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
			PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Read instruct of %s.%s cell @[%03u:%02u] Failed.", xls_fullpathname, wsname.c_str(), r, 1);
			return err;
		}

		if (tmp_cell_str.size() == 1 && tmp_cell_str.at(0) == '$') {
			if (xls_config_generator::parse_phase_enum::col_spec == pe) {
				PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Got var instruct section at %s.%s @[%03u:%02u], phase is %d ", xls_fullpathname, wsname.c_str(), r, 1, (int)pe);
				return err;
			} else if (xls_config_generator::parse_phase_enum::row_data == pe) {
				err = ws.strvalue(tmp_varname_cell, r, 2);
				::pilo::core::string::trim_string(tmp_varname_cell);
				if (::pilo::core::string::i_compare(tmp_varname_cell.c_str(), 0, "END", 0, -1) == 0) {
					PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::info, "Read data finished: %s.%s.", xls_fullpathname, wsname.c_str());
					break;
				} else {
					PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Got instruct at %s.%s @[%03u:%02u], phase is %d, but cmd is not /END/.", xls_fullpathname, wsname.c_str(), r, 1, (int)pe);
					return err;
				}						
			}


			err = ws.strvalue(tmp_varname_cell, r, 2);
			::pilo::core::string::trim_string(tmp_varname_cell);
			if (err != PILO_OK) {
				PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Read var of %s.%s cell @[%03u:%02u] Failed.", xls_fullpathname, wsname.c_str(), r, 2);
				return err;
			}
			::pilo::i64_t idx_tmp = ::pilo::core::string::split(strkvpair, 2, tmp_varname_cell, ':', true);
			if (idx_tmp < 1 || idx_tmp > 2) {
				PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Format of var of %s.%s cell @[%03u:%02u] Failed. shoudl be /key:value/", xls_fullpathname, wsname.c_str(), r, 2);
				return err;
			}

			if (::pilo::core::string::i_compare(strkvpair[0].c_str(), 0, "DESC", 0, -1) == 0) {
				desc = strkvpair[1];				
			} else if (::pilo::core::string::i_compare(strkvpair[0].c_str(), 0, "CCNAME", 0, -1) == 0) {
				ccname = strkvpair[1];
				if (ccname.empty()) {
					PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::warn, "CCNAME (%s) not set, client class file wont be generated for %s.%s", ccname.c_str(), xls_fullpathname, wsname.c_str());
				}
			} else if (::pilo::core::string::i_compare(strkvpair[0].c_str(), 0, "SCNAME", 0, -1) == 0) {
				scname = strkvpair[1];
				if (scname.empty()) {
					PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::warn, "SCNAME (%s) not set, server class file wont be generated for %s.%s", scname.c_str(), xls_fullpathname, wsname.c_str());
				}
			} else if (::pilo::core::string::i_compare(strkvpair[0].c_str(), 0, "CSRC", 0, -1) == 0) {
				csrc = strkvpair[1];
				if (csrc.empty()) {
					PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "CSRC (%s) is Assigned, but empty, cant continue. %s.%s", scname.c_str(), xls_fullpathname, wsname.c_str());
					return PERR_INC_DATA;
				}
			} else if (::pilo::core::string::i_compare(strkvpair[0].c_str(), 0, "SSRC", 0, -1) == 0) {
				ssrc = strkvpair[1];
				if (ssrc.empty()) {
					PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "SSRC (%s) is Assigned, but empty, cant continue. %s.%s", scname.c_str(), xls_fullpathname, wsname.c_str());
					return PERR_INC_DATA;
				}
			} else if (::pilo::core::string::i_compare(strkvpair[0].c_str(), 0, "CCFG", 0, -1) == 0) {
				ccfg = strkvpair[1];
				if (ccfg.empty()) {
					PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::warn, "CCFG (%s) not set, client data file wont be generated for %s.%s", ccfg.c_str(), xls_fullpathname, wsname.c_str());
				}
			} else if (::pilo::core::string::i_compare(strkvpair[0].c_str(), 0, "SCFG", 0, -1) == 0) {
				scfg = strkvpair[1];
				if (scfg.empty()) {
					PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::warn, "SCFG (%s) not set, server data file wont be generated for %s.%s", scfg.c_str(), xls_fullpathname, wsname.c_str());
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
				PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Varname of %s.%s cell @[%03u:%02u] not found.", xls_fullpathname, wsname.c_str(), r, 2);
				return PERR_INC_DATA;
			}
			
		} else if (tmp_cell_str.size() == 1 && (tmp_cell_str.at(0) == 'C' || tmp_cell_str.at(0) == 'c')) {
			if (xls_config_generator::parse_phase_enum::row_data == pe) {
				PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Got client-col-spec instruct section at %s.%s @[%03u:%02u], phase is %d ", xls_fullpathname, wsname.c_str(), r, 1, (int)pe);
				return PERR_INC_DATA;
			} else if (xls_config_generator::parse_phase_enum::header_vars == pe) {
				if (!_check_duplicate_vars_in_header(ccname, scname, ccfg, scfg, xls_fullpathname, wsname.c_str()))
					return PERR_INC_DATA;
				pe = xls_config_generator::parse_phase_enum::col_spec;				
			}

			::pilo::u32_t field_tmp_cnt = ws.col_count();
			if (field_tmp_cnt < 1) {
				PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Client Fields Spec Got 0 or minus Count (row=%u) of %s.%s", r, xls_fullpathname, wsname.c_str());
				return ::pilo::mk_perr(PERR_INC_DATA);
			}
			::pilo::u32_t field_real_cnt = 0;			
			for (::pilo::u32_t c = 2; c <= field_tmp_cnt; c++) {				
				err = ws.strvalue(tmp_fld_string, r, c);
				if (err != PILO_OK) {
					PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Client Fields Spec Read Failed @[%03u:%02u]  of %s.%s", r, c, xls_fullpathname, wsname.c_str());
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
				PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Client Fields Spec Count Mismatch %u : %u,  %s.%s", (::pilo::u32_t) cfg_set._configs[xls_config_set::client]._fields.size() , field_real_cnt, xls_fullpathname, wsname.c_str());
				return PERR_INC_DATA;
			}
			
			c_header_done = true;

			if (s_header_done)
				pe = xls_config_generator::parse_phase_enum::row_data;

						

		} else if (tmp_cell_str.size() == 1 && (tmp_cell_str.at(0) == 'S' || tmp_cell_str.at(0) == 's')) {
			if (xls_config_generator::parse_phase_enum::row_data == pe) {
				PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Got server-col-spec instruct section at %s.%s @[%03u:%02u], phase is %d ", xls_fullpathname, wsname.c_str(), r, 1, (int)pe);
				return PERR_INC_DATA;
			} else if (xls_config_generator::parse_phase_enum::header_vars == pe) {
				if (!_check_duplicate_vars_in_header(ccname, scname, ccfg, scfg, xls_fullpathname, wsname.c_str()))
					return PERR_INC_DATA;
				pe = xls_config_generator::parse_phase_enum::col_spec;

			}
			::pilo::u32_t field_tmp_cnt = ws.col_count();
			if (field_tmp_cnt < 1) {
				PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Server Fields Spec Got 0 or minus Count (row=%u) of %s.%s", r, xls_fullpathname, wsname.c_str());
				return ::pilo::mk_perr(PERR_INC_DATA);
			}

			::pilo::u32_t field_real_cnt = 0;
			for (::pilo::u32_t c = 2; c <= field_tmp_cnt; c++) {
				err = ws.strvalue(tmp_fld_string, r, c);
				if (err != PILO_OK) {
					PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Server Fields Spec Read Failed @[%03u:%02u]  of %s.%s", r, c, xls_fullpathname, wsname.c_str());
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
				PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Server Fields Spec Count Mismatch %u : %u,  %s.%s", (::pilo::u32_t)cfg_set._configs[xls_config_set::server]._fields.size(), field_real_cnt, xls_fullpathname, wsname.c_str());
				return PERR_INC_DATA;
			}

			s_header_done = true;

			if (c_header_done) 
				pe = xls_config_generator::parse_phase_enum::row_data;			


		}
		else if (tmp_cell_str.size() > 0 && tmp_cell_str.at(0) == '/' && tmp_cell_str.at(1) == '/') {
			continue;
		} else if (tmp_cell_str.empty()) {
			if (xls_config_generator::parse_phase_enum::row_data != pe) {
				if (xls_config_generator::parse_phase_enum::col_spec == pe) {
					if (! s_header_done && ! c_header_done) {
						PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "No Header Specs Found before data begins: %s.%s", xls_fullpathname, wsname.c_str());
						return err;
					} else if (!s_header_done) {
						PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::warn, "No Server Header Specs Found!, still we begin with client: %s.%s", xls_fullpathname, wsname.c_str());
						pe = xls_config_generator::parse_phase_enum::row_data;
					} else if (!c_header_done) {
						PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::warn, "No Client Header Specs Found!, still we begin with server: %s.%s", xls_fullpathname, wsname.c_str());
						pe = xls_config_generator::parse_phase_enum::row_data;
					}

				} else {
					PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Got empty instruct section at %s.%s @[%03u:%02u], phase is %d ", xls_fullpathname, wsname.c_str(), r, 1, (int)pe);
					return err;
				}
				
			}

			if (s_header_done && cfg_set._configs[xls_config_set::server].field_count() > 0) {
				err = this->_parse_record(cfg_set, xls_config_set::server, r, &ws, xls_fullpathname, wsname.c_str());
				if (err != PILO_OK) {
					return err;
				}
			}

			
			if (c_header_done && cfg_set._configs[xls_config_set::client].field_count() > 0) {
				err = this->_parse_record(cfg_set, xls_config_set::client, r, &ws, xls_fullpathname, wsname.c_str());
				if (err != PILO_OK) {
					return err;
				}
			}
			
		}

		
			
		
	} //end of while
	
	if (cfg_set._configs[xls_config_set::server].field_count() < 1) {
		if (!cfg_set._configs[xls_config_set::server].check_uniqe(errbuff, PMI_XLS_GEN_ERR_BUFF_SIZE)) {
			PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Check Unique Failed against Server data in %s.%s.  (%s) ", xls_fullpathname, wsname.c_str(), errbuff);
			return ::pilo::mk_perr(PERR_INC_DATA);
		}
	}
	
	if (cfg_set._configs[xls_config_set::client].field_count() < 1) {
		if (!cfg_set._configs[xls_config_set::client].check_uniqe(errbuff, PMI_XLS_GEN_ERR_BUFF_SIZE)) {
			PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "Check Unique Failed against Client data in %s.%s.  (%s) ", xls_fullpathname, wsname.c_str(), errbuff);
			return ::pilo::mk_perr(PERR_INC_DATA);
		}
	}
	

	cfg_set._desc = desc;
	cfg_set._sheet_name = wsname;
	cfg_set._xls_name = xls_fullpathname;
	cfg_set._configs[xls_config_set::server]._cls_name = scname;
	cfg_set._configs[xls_config_set::server]._config_file_name = scfg;
	cfg_set._configs[xls_config_set::server]._ns = sns;
	cfg_set._configs[xls_config_set::client]._cls_name = ccname;
	cfg_set._configs[xls_config_set::client]._config_file_name = ccfg;
	cfg_set._configs[xls_config_set::client]._ns = cns;
	if (ssrc.empty())
		cfg_set._configs[xls_config_set::server]._source_file_name = scname;
	else
		cfg_set._configs[xls_config_set::server]._source_file_name = ssrc;
	if (csrc.empty())
		cfg_set._configs[xls_config_set::client]._source_file_name = ccname;
	else
		cfg_set._configs[xls_config_set::client]._source_file_name = csrc;

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
		PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "xls_conf_gen@0x%p Already set!", this);
		return PERR_EXIST;
	}

	if (nullptr == xls_dir_path) {
		PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::error, "xls_conf_gen@0x%p xls dir path is null", this);
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
	_log_set.clear();
	_ccnames.clear();
	_scnames.clear();
	_ccfgs.clear();
	_scfgs.clear();
}

::pilo::err_t pilo::core::config::xls_config_generator::parse()
{
	PMF_APPEND_TEXT_LOG_DFL(_log_set, ::pilo::core::logging::level::info, "Start Parsing dir - [%s]", _xls_dir_path.fullpath());

	::pilo::err_t ret = ::pilo::core::io::path::dfs_travel_path(&_xls_dir_path, s_xls_file_iter_func, this, false, ::pilo::core::io::path::evt_node_visiting);
	if (ret != PILO_OK) {
		return ret;
	}

	std::stringstream ss;
	for (std::map<std::string, xls_config_set>::const_iterator cit = _config_set_map.cbegin(); cit != _config_set_map.cend(); cit ++) {
		ss.clear();
		ss.str("");
		ss << cit->first << "\n" << cit->second.to_string();
		_log_set.append_text_item(::pilo::core::logging::level::debug, ss.str(), __FILE__, __LINE__);
	}

	

	return PILO_OK;
}

::pilo::err_t pilo::core::config::xls_config_generator::generate_server_config()
{
	return _generate_config(xls_config_set::server);
}

::pilo::err_t pilo::core::config::xls_config_generator::generate_client_config()
{

	return _generate_config(xls_config_set::client);
}

::pilo::err_t pilo::core::config::xls_config_generator::generate_server_source(::pilo::core::autogen::lang_type ltype)
{

	return _generate_source(xls_config_set::server, ltype);
}

::pilo::err_t pilo::core::config::xls_config_generator::generate_client_source(::pilo::core::autogen::lang_type ltype)
{

	return _generate_source(xls_config_set::client, ltype);
}


