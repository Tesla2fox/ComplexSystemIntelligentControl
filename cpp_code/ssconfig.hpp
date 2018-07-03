#ifndef _SSCONFIG_H
#define _SSCONFIG_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <map>
#include <cmath>

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

namespace sscfg{
	// '#' is for comment
	// blank spaces are used as seperators
	// the first str in a line is the name of the items in 
	// do not use comma here
	template<class T>
	inline T str2dat(const std::string& str, bool* ok = 0){
		T res;
		std::stringstream ss(str);
		ss >> res;
		if (ok) *ok = !ss.fail();
		return res;
	}
	
	template<>
	inline std::string str2dat(const std::string& str, bool* ok){
		if (ok) *ok = true;
		return str;
	}
	
	template<>
	inline bool str2dat<bool>(const std::string& str, bool* ok){
		if (str == "true" || str == "True" || str == "TRUE")    return true;
		if (str == "false" || str == "False" || str == "FALSE") return false;
		int s;
		std::stringstream ss(str);
		ss >> s;

		if (ok) *ok = !ss.fail();
		
		return s != 0;
	}
	 
	class ConfigFile{
	public:
		static ConfigFile load(const std::string& fname){
			std::ifstream fin(fname);
			return load(fin);
		}
		static ConfigFile load(std::istream& fin);
		
		bool exist(const std::string& name){
			return (dict.find(name) != dict.end());
		}
		size_t n_items(const std::string& name){
			std::map<std::string, Range>::iterator iter = dict.find(name);
			if (iter != dict.end()){
				return iter->second.size();
			}
			return 0;
		}
		
		template<class T>
		bool get(const std::string& name, T& res);
		
		template<class T>
		inline size_t get(const std::string& name, T* res, size_t num_max);
		
		//template<>
		//inline size_t get<char>(const std::string& name, char* res, size_t num_max);
		
		template<class T>
		size_t get(const std::string& name, std::vector<T>& res);
		
	private:
		struct Range{
			bool empty() const  {return end == beg;}
			size_t size() const {return end - beg;}
			size_t beg, end;
		};
        
        static std::istream& safeGetline(std::istream& is, std::string& t){
            t.clear();

            // The characters in the stream are read one-by-one using a std::streambuf.
            // That is faster than reading them one-by-one using the std::istream.
            // Code that uses streambuf this way must be guarded by a sentry object.
            // The sentry object performs various tasks,
            // such as thread synchronization and updating the stream state.

            std::istream::sentry se(is, true);
            std::streambuf* sb = is.rdbuf();

            for(;;) {
                int c = sb->sbumpc();
                switch (c) {
                case '\n':
                    return is;
                case '\r':
                    if(sb->sgetc() == '\n')
                        sb->sbumpc();
                    return is;
                case EOF:
                    // Also handle the case when the last line has no line ending
                    if(t.empty())
                        is.setstate(std::ios::eofbit);
                    return is;
                default:
                    t += (char)c;
                }
            }
        }
        
		std::vector<std::string>     strlist;
		std::map<std::string, Range> dict;
	};
	
	inline ConfigFile ConfigFile::load(std::istream& fin){
		ConfigFile db;
		std::string line;
		std::string id;
		std::string s;
		Range range;
		
		if (!fin) return db;
		while (safeGetline(fin, line)){
			if (line.empty())
				continue;
			std::stringstream ss(line);
			id.clear();
			ss >> id;
			if (id.empty()){
				// This line is blank, ignore
				continue;
			}			
			if (id[0] == '#') {
				// This line is comment line, ignore
				continue;
			}
			range.beg = db.strlist.size();
			range.end = range.beg;
			while (ss >> s){
				if (s[0] != '#'){
					db.strlist.push_back(s);
					range.end++;
				}
				else{
					// Get inline comment
					break;
				}
			}
			db.dict[id] = range;
		}
		
		return db;
	}
	
	
	template<class T>
	inline bool ConfigFile::get(const std::string& name, T& res){
		std::map<std::string, Range>::iterator iter = dict.find(name);
		if (iter != dict.end()){
			Range& rg = iter->second;
			if (!rg.empty()){
				res = str2dat<T>(strlist[rg.beg]);
			}
			return true;
		}
		else{
			return false;
		}
	}
	
	template<class T>
	inline size_t ConfigFile::get(const std::string& name, T* res, size_t num_max){
		std::map<std::string, Range>::iterator iter = dict.find(name);
		if (iter != dict.end()){
			Range& rg = iter->second;
			if (!rg.empty() && num_max > 0){
				size_t n = 0;
				size_t sz = rg.size();
				while (n < num_max && n < sz){
					*res = str2dat<T>(strlist[rg.beg + n]);
					res++;
					n++;
				}
				return n;
			}
			return 0;
		}
		else{
			return 0;
		}
	}
	
	template<>
	inline size_t ConfigFile::get<char>(const std::string& name, char* res, size_t num_max){
		std::map<std::string, Range>::iterator iter = dict.find(name);
		if (iter != dict.end()){
			Range& rg = iter->second;
			if (!rg.empty() && num_max > 0){
				std::string& s = strlist[rg.beg];
				const char* ss = s.c_str();
				
				size_t N = num_max - 1;
				if (N > s.size()) 
					N = s.size();
				for (size_t n=0; n < N; ++n){
					res[n] = ss[n];
				}
				res[N] = 0;
				return N + 1;
			}
			else{
				return 0;    
			}
		}
		else{
			return 0;
		}
	}
	
	
	template<class T>
	size_t ConfigFile::get(const std::string& name, std::vector<T>& res){
		std::map<std::string, Range>::iterator iter = dict.find(name);
		if (iter != dict.end()){
			Range& rg = iter->second;
			if (!rg.empty()){
				res.reserve(res.size() + rg.size());
				
				size_t n = 0;
				while (n < rg.size()){
					res.push_back(str2dat<T>(strlist[rg.beg + n]));
					n++;
				}
				return n;
			}
			return 0;
		}
		else{
			return 0;
		}
	}

	
	class ConfigSet{
	public:
		ConfigSet(){}
		
		enum{
			Good = 0, InvalidType, NameExist
		};
		enum {
			ArgGood, ArgNotExist, ArgUnhandled, ArgInvalid
		};

		template<class T>
		int addarg(T& arg, const std::string& name){
			if (dat.find(name) != dat.end()) return NameExist;
			ArgItem item;
			item.ptr = (void*)(&arg);
			item.result = ArgUnhandled;
			item.type = getDataType(arg);
			if (item.type == DataUnsupported)
				return InvalidType;

			dat[name] = item;
			return Good;
		}

		int loadline(const std::string& name, const std::string& value){
			bool ok;
			if (dat.find(name) != dat.end()){
				ArgItem& it = dat[name];
#define fast_get( opt, T ) if (it.type == opt) { T v = str2dat< T >( value, &ok ); if (ok) *reinterpret_cast< T* >(it.ptr) = v; }
				fast_get(DataFloat, float);
				fast_get(DataDouble, double);
				fast_get(DataInt, int);
				fast_get(DataBool, bool);
				fast_get(DataString, std::string);
#undef fast_get
			}
			else{
				return ArgNotExist;
			}
		}
		
		int load(ConfigFile& file){
			int counter_invalid = 0;
			std::map<std::string, ArgItem>::iterator iter = dat.begin();
			for (; iter != dat.end(); iter++){
				const std::string& name = iter->first;
				ArgItem& it = iter->second;
#define fast_load( opt, T ) if (it.type == opt) { \
	T v; if (file.get(name, v) == true) { it.result = ArgGood; *reinterpret_cast< T* >(it.ptr) = v; } \
		else { it.result = ArgInvalid; counter_invalid++; std::cout<< "Failed to Load " << name << "\n"; } \
}                
				fast_load(DataFloat,  float);
				fast_load(DataDouble, double);
				fast_load(DataInt,    int);
				fast_load(DataBool,   bool);
				fast_load(DataString, std::string);
				//fast_load(DataStdVecFloat, std::vector<float>);
#undef fast_load
			}
			return counter_invalid;
		}
		
		int load(std::istream& in){
			ConfigFile file = ConfigFile::load(in);
			return load(file);
		}
		
		int load_file(const std::string& file_name){
			std::ifstream file(file_name.c_str());
			if (file.is_open())
				return load(file);
			return dat.size();
		}
		
	protected:
		enum {
			DataFloat, DataDouble, DataInt, DataString, DataUnsupported, DataBool,
			DataStdVecFloat
		};
		
		template<class T>
		int getDataType(T& arg) { return DataUnsupported; }
		int getDataType(float& arg){ return DataFloat;	}
		int getDataType(double& arg){ return DataDouble; }
		int getDataType(int& arg){ return DataInt; }
		int getDataType(std::string& arg){ return DataString; }
		int getDataType(bool& arg) {return DataBool; }
		int getDataType(std::vector<float>& arg){ return DataStdVecFloat; }

		struct ArgItem{
			void*  ptr;
			int result;
			int type;
		};
		std::map<std::string, ArgItem> dat;
	};

}

#endif
