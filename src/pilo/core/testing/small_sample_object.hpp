#pragma once

#include "../../pilo.hpp"
#include <string>

namespace pilo
{
	namespace core
	{
		namespace testing
		{
			class small_sample_object
			{
			public:
				small_sample_object()
				{
					_id = -1;
					_name.clear();
				}

			public:
				void set(::pilo::i64_t id) 
				{
					_id = id;
					_name = "small_sample_object" + _id;
				}
				bool validate(::pilo::i64_t cv) const
				{
					if (_id == -1)
					{
						return false;
					}

					if (cv == -1) 
					{
						std::string name = "small_sample_object" + _id;
						if (_name == name) 
						{
							return true;
						}						
					}
					else
					{
						std::string name = "small_sample_object" + cv;
						if (_name == name)
						{
							return true;
						}
					}
					return false;

				}
				::pilo::i64_t id() const { return _id; }
				std::string name() const { return _name;  }
				const char* cname() const { return _name.c_str(); }

			private:
				::pilo::i64_t _id;
				std::string _name;

			};
		}
	}
}