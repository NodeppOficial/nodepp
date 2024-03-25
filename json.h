/**
*
*	tiny::TinyJson library
*	Copyright 2017 Button
*
*/

#ifndef TINY_JSON_H_
#define TINY_JSON_H_

#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>

namespace tiny {

	/**
	* 无类型，解析时确认
	*
	*/
	class Value
	{
	public:
		Value() {
			value_.clear();
			nokey_ = false;
		}
		Value(std::string val) : value_(val) {
			if (value_ == "") {
				value_.clear();
				nokey_ = true;
			}
			else {
				nokey_ = false;
			}
		}
		~Value() {}

	public:
		std::string value() { return value_; }
		template<typename R> 
		R GetAs() {
			std::istringstream iss(value_);
			R v;
			iss >> v;
			return v;
		}


		template<typename V> 
		void Set(V v) {
			std::ostringstream oss;
			if (nokey_) {
				oss << v;
			}
			else {
				oss << "\"" << value_ << "\"" << ":" << v;
			}
			value_ = oss.str();
		}

		template<typename T>
		void Push(T& v) {
			std::ostringstream oss;
			if (v.get_nokey()) {
                oss << v.WriteJson(0);
			}
			else {
                oss << v.WriteJson(1);
			}
			value_ = oss.str();
		}

	private:
		std::string value_;
		bool nokey_;
	};

        template<> inline bool Value::GetAs() { return value_ == "true" ? true : false; }
        template<> inline std::string Value::GetAs() { return value_; }
        template<>
        inline void Value::Set(std::string v) {
                std::ostringstream oss;
                if (nokey_) {
                        oss << "\"" << v << "\"";
                }
                else {
                        oss << "\"" << value_ << "\"" << ":" << "\"" << v << "\"";
                }
                value_ = oss.str();
        }

        template<>
        inline void Value::Set(const char* v) {
                Set(std::string(v));
        }

        template<>
        inline void Value::Set(bool v) {
                std::ostringstream oss;
                std::string val = v == true ? "true" : "false";
                if (nokey_) {
                        oss << val;
                }
                else {
                        oss << "\"" << value_ << "\"" << ":" << val;
                }
                value_ = oss.str();
        }

	/**
	* 此模板类处理json键对应的值是一个嵌套对象或者数组的情况
	* 
	*/
	template<typename T>
	class ValueArray : public T
	{
	public:
		ValueArray() {}
		ValueArray(std::vector<std::string> vo) { vo_ = vo; }

		bool Enter(int i) {
			std::string obj = vo_[i];
                        return this->ReadJson(obj);
		}

		int Count() { return vo_.size(); }

	private:
		std::vector<std::string> vo_;
	};

	/**
	* 解析json字符串保存为键值的顺序存储，解析是按一层一层的进行
	* 解析时把json看做是对象'{}' 与 数组'[]' 的组合
	*
	*/
	class ParseJson
	{
	public:
        ParseJson() {}
        ~ParseJson() {}

	public:
		bool ParseArray(std::string json, std::vector<std::string>& vo);
		bool ParseObj(std::string json);
		std::vector<std::string> GetKeyVal() {
			return keyval_;
		}

	protected:
		std::string Trims(std::string s, char lc, char rc);
		int GetFirstNotSpaceChar(std::string& s, int cur);
		std::string FetchArrayStr(std::string inputstr, int inpos, int& offset);
		std::string FetchObjStr(std::string inputstr, int inpos, int& offset);
		std::string FetchStrStr(std::string inputstr, int inpos, int& offset);
		std::string FetchNumStr(std::string inputstr, int inpos, int& offset);

	private:
		std::vector<char> token_;
		std::vector<std::string> keyval_;
	};

	inline bool ParseJson::ParseArray(std::string json, std::vector<std::string>& vo) {
		json = Trims(json, '[', ']');
		std::string tokens;
		size_t i = 0;
		for (; i < json.size(); ++i) {
			char c = json[i];
			if (isspace(c) || c == '\"') continue;
			if (c == ':' || c == ',' || c == '{') {
				if (!tokens.empty()) {
					vo.push_back(tokens);
					tokens.clear();
				}
				if (c == ',') continue;
				int offset = 0;
				char nextc = c;
				for (; c != '{';) {
					nextc = json[++i];
					if (isspace(nextc)) continue;
					break;
				}
				if (nextc == '{') {
					tokens = FetchObjStr(json, i, offset);
				}
				else if (nextc == '[') {
					tokens = FetchArrayStr(json, i, offset);
				}
				i += offset;
				continue;
			}
			tokens.push_back(c);
		}
		if (!tokens.empty()) {
			vo.push_back(tokens);
		}
		return true;
	}

	// 解析为 key-value 调用一次解析一个层次
	inline bool ParseJson::ParseObj(std::string json) {
		auto LastValidChar = [&](int index)->char{
			for (int i = index-1; i >= 0; --i){
				if (isspace(json[i])) continue;
				char tmp = json[i];
				return tmp;
			}
			return '\0';
		};

		json = Trims(json, '{', '}');
		size_t i = 0;
		for (; i < json.size(); ++i) {
			char nextc = json[i];
			if (isspace(nextc)) continue;

			std::string tokens;
			int offset = 0;
			if (nextc == '{') {
				tokens = FetchObjStr(json, i, offset);
			}
			else if (nextc == '[') {
				tokens = FetchArrayStr(json, i, offset);
			}
			else if (nextc == '\"') {
				tokens = FetchStrStr(json, i, offset);
			}
			else if (( isdigit(nextc) || nextc == '-') && LastValidChar(i) == ':')
			{
				tokens = FetchNumStr(json, i, offset);
			}
			else {
				continue;
			}
			keyval_.push_back(tokens);
			i += offset;
		}
		if (keyval_.size() == 0)
		{
			keyval_.push_back(json);
		}
		return true;
	}

	inline std::string ParseJson::Trims(std::string s, char lc, char rc)
	{
		std::string ss = s;
		if (s.find(lc) != std::string::npos && s.find(rc) != std::string::npos) {
			size_t b = s.find_first_of(lc);
			size_t e = s.find_last_of(rc);
			ss = s.substr(b + 1, e - b - 1);
		}
		return ss;
	}

	inline int ParseJson::GetFirstNotSpaceChar( std::string& s, int cur )
	{
		for (size_t i = cur; i < s.size(); i++){
			if (isspace(s[i])) continue;
			return i - cur;
		}
		return 0;
	}

	inline std::string ParseJson::FetchArrayStr(std::string inputstr, int inpos, int& offset)
	{
		int tokencount = 0;
		std::string objstr;
		size_t i = inpos + GetFirstNotSpaceChar(inputstr, inpos);
		for (; i < inputstr.size(); i++) {
			char c = inputstr[i];
			if (c == '[') {
				++tokencount;
			}
			if (c == ']') {
				--tokencount;
			}
			objstr.push_back(c);
			if (tokencount == 0) {
				break;
			}
		}
		offset = i - inpos;
		return objstr;
	}

	inline std::string ParseJson::FetchObjStr(std::string inputstr, int inpos, int& offset)
	{
		int tokencount = 0;
		std::string objstr;
		size_t i = inpos + GetFirstNotSpaceChar(inputstr, inpos);
		for (; i < inputstr.size(); i++) {
			char c = inputstr[i];
			if (c == '{') {
				++tokencount;
			}
			if (c == '}') {
				--tokencount;
			}
			objstr.push_back(c);
			if (tokencount == 0) {
				break;
			}
		}
		offset = i - inpos;
		return objstr;
	}

	inline std::string ParseJson::FetchStrStr( std::string inputstr, int inpos, int& offset )
	{
		int tokencount = 0;
		std::string objstr;
		size_t i = inpos + GetFirstNotSpaceChar(inputstr, inpos);
		for (; i < inputstr.size(); i++) {
			char c = inputstr[i];
			if (c == '\"') {
				++tokencount;
			}
			objstr.push_back(c);
			if (tokencount % 2 == 0 && (c == ',' || c == ':')) {
				break;
			}
		}
		offset = i - inpos;

		return Trims(objstr, '\"', '\"');
	}

	inline std::string ParseJson::FetchNumStr( std::string inputstr, int inpos, int& offset )
	{
		std::string objstr;
		size_t i = inpos + GetFirstNotSpaceChar(inputstr, inpos);
		for (; i < inputstr.size(); i++) {
			char c = inputstr[i];
			if (c == ',') {
				break;
			}
			objstr.push_back(c);
		}
		offset = i - inpos;

		return objstr;
	}

	/**
	* 对外接口类
	*
	*/
	class TinyJson;
	typedef ValueArray<TinyJson> xarray;
	typedef ValueArray<TinyJson> xobject;

	class TinyJson
	{
		friend class ValueArray<TinyJson>;
	public:
		TinyJson() {
			nokey_ = false;
		}
        ~TinyJson() {}

	public:
		// read
		bool ReadJson(std::string json) {
			ParseJson p;
			p.ParseObj(json);
			KeyVal_ = p.GetKeyVal();
			return true;
		}

		template<typename R>
		R Get(std::string key, R defVal) {
			auto itr = std::find(KeyVal_.begin(), KeyVal_.end(), key);
			if (itr == KeyVal_.end()) {
				return defVal;
			}
			return Value(*(++itr)).GetAs<R>();
		}

		template<typename R>
		R Get(std::string key) {
			return Get(key, R());
		}

        template<typename R>
        R Get() {
            return Value(KeyVal_[0]).GetAs<R>();
        }

		// write
		Value& operator[](std::string k) {
			Items_.push_back(Value(k));
			Value& v = Items_[Items_.size() - 1];
			if (k == "") {
				nokey_ = true;
			}
			return v;
		}

		void Push(TinyJson item) {
			Items_.push_back(Value(""));
			Value& v = Items_[Items_.size() - 1];
			nokey_ = true;
			v.Push(item);
			sub_type_ = 1;
		}

		bool get_nokey() {
			return nokey_;
		}

		std::string WriteJson(){
            return WriteJson(1);
		}

        // 0: none  1: object  2: array
        std::string WriteJson(int type);

	public:
		int sub_type_;

	private:
		std::vector<std::string> KeyVal_;
		std::vector<Value> Items_;
		bool nokey_;
	};

    template<>
    inline xarray TinyJson::Get(std::string key) {
        std::string val = Get<std::string>(key);
        ParseJson p;
        std::vector<std::string> vo;
        p.ParseArray(val, vo);
        xarray vals(vo);
        return vals;
    }

    inline std::ostream & operator << (std::ostream& os, TinyJson& ob)
	{
		os << ob.WriteJson();
		return os;
	}

    inline std::string TinyJson::WriteJson(int type)
	{
        std::string prefix = type == 1 ? "{" : "[";
        std::string suffix = type == 1 ? "}" : "]";
        if (type == 0) {
			prefix = "";
			suffix = "";
		}
		std::ostringstream oss;
		oss << prefix;
		int i = 0;
		int size = Items_.size();
		std::string seq = ",";
		for (; i < size; ++i) {
			Value& v = Items_[i];
			oss << v.value() << seq;
		}
		std::string jsonstring = oss.str();
		if (jsonstring.back() == ',') {
			jsonstring = jsonstring.substr(0, jsonstring.find_last_of(','));
		}

		jsonstring += suffix;
		return jsonstring;
	}

	template<>
	inline void Value::Set(TinyJson v) {
		std::ostringstream oss;
		if (v.sub_type_ == 1) {
            oss << "\"" << value_ << "\"" << ":" << v.WriteJson(2);
		}
		else {
			if (nokey_) {
				oss << v;
			}
			else {
				oss << "\"" << value_ << "\"" << ":" << v;
			}
		}
		value_ = oss.str();
	}

}  // end namesapce

#endif  // TINY_JSON_H_