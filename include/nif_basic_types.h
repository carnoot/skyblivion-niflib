/* Copyright (c) 2006, NIF File Format Library and Tools
All rights reserved.  Please see niflib.h for license. */

#ifndef _NIF_BASIC_TYPES_H_
#define _NIF_BASIC_TYPES_H_

#include <string>
#include <stdarg.h>
#include <vector>
#include "gen/enums.h"
#include "nif_versions.h"
#include <array>

namespace Niflib {

	template<typename C>
	struct is_iterable
	{
		typedef long false_type;
		typedef char true_type;

		template<class T> static false_type check(...);
		template<class T> static true_type  check(int,
			typename T::const_iterator = C().end());

		enum { value = sizeof(check<C>(0)) == sizeof(true_type) };
	};

	template <template <typename...> class Base, typename Derived>
	struct is_base_of_template
	{
		using U = typename std::remove_cv<Derived>::type;

		template <typename... Args>
		static std::true_type test(Base<Args...>*);

		static std::false_type test(void*);

		using type = decltype(test(std::declval<U*>()));
	};

	template <template <typename...> class Base, typename Derived>
	using is_base_of_template_t = typename is_base_of_template<Base, Derived>::type;

using namespace std;

struct NifInfo;

struct Native {};
struct Compound {};
struct HalfVector3;

struct HeaderString : public Native {
	string header;

	bool operator==(const HeaderString& other) const { return header == other.header; }

};

struct ShortString : public Native {
	string str;

	bool operator==(const ShortString& other) const { return str == other.str; }
};

struct LineString : public Native {
	string line;

	bool operator==(const LineString& other) const { return line == other.line; }
};

struct IndexString : public std::string, public Native {
	IndexString() {}
	IndexString( const IndexString & ref ) : std::string((std::string const &)ref) {}
	IndexString( const std::string & ref ) : std::string(ref) {}
	IndexString& operator=( const IndexString & ref ) { assign((std::string const &)ref); return *this; }
	IndexString& operator=( const std::string & ref ) { assign(ref); return *this; }
	operator std::string const &() const { return *this; }
	operator std::string &() { return *this; }
};

struct Char8String : public std::string, public Native {
	Char8String() {}
	Char8String( const Char8String & ref ) : std::string((std::string const &)ref) {}
	Char8String( const std::string & ref ) : std::string(ref) {}
	Char8String& operator=( const Char8String & ref ) { assign((std::string const &)ref); return *this; }
	Char8String& operator=( const std::string & ref ) { assign(ref); return *this; }
	operator std::string const &() const { return *this; }
	operator std::string &() { return *this; }
};

struct ByteColor4 : public Native {
	union {
		uint32_t desc = 0;
		struct {
			/*! Red color component. */
			uint8_t r;
			/*! Green color component. */
			uint8_t g;
			/*! Blue color component. */
			uint8_t b;
			/*! Alpha color component. */
			uint8_t a;
		};
	};

	bool operator==(const ByteColor4& other) const { return desc == other.desc; }
};

//--Non-mathematical Basic Types--//
#ifndef byte
typedef unsigned char byte;
#endif

/*! An array of bytes. */

struct ByteArray : public std::vector<Niflib::byte>, public Compound {};
//typedef std::vector<Niflib::byte> ByteArray;

// Fallout 4 Declaration of half float
#ifndef hfloat
struct hfloat { 

	unsigned short value = 0;

	hfloat() {};
	hfloat(unsigned short val) { value = val; };
	hfloat& operator=(const unsigned short & ref) {value = ref; return *this; }
	bool operator==(const hfloat& other) const { return value == other.value; }
};
//typedef unsigned short	hfloat;
#endif

//--Structures--//

/*! 
 * Used to specify optional ways the NIF file is to be written or retrieve information about
 * the way an existing file was stored. 
 */
struct NifInfo {
	NifInfo() : version(VER_4_0_0_2), userVersion(0), userVersion2(0), endian(ENDIAN_LITTLE) {}
	NifInfo( unsigned version, unsigned userVersion = 0, unsigned userVersion2 = 0) {
		this->version = version;
		this->userVersion = userVersion;
		this->userVersion2 = userVersion2;
		endian = ENDIAN_LITTLE;
	}
	unsigned version;
	unsigned userVersion;
	unsigned userVersion2;
	/*! Specifies which low-level number storage format to use. Should match the processor type for the target system. */
	EndianType endian;
	/*! This is only supported in Oblivion.  It contains the name of the person who created the NIF file. */
	string author;
	/*! This is only supported in Oblivion.  It seems to contain the type of script or program used to export the file. */
	string processScript;
	/*! This is only supported in Oblivion.  It seems to contain the more specific script or options of the above. */
	string exportScript;
};

//Need to use an iterable class
template<size_t size, class T>
using array = std::array<T, size>;

///*! Used to enable static arrays to be members of vectors */
//template<int size, class T>
//struct array {
//	array() {
//		for ( size_t i = 0; i < size; ++i )
//			data[i] = T();
//	}
//// XXX ellipsis does not work when T = float
//// XXX see for instance http://support.microsoft.com/kb/71424
///*
//	array(size_t n, ...) {
//		va_list argptr;
//		va_start(argptr, n);
//		for ( size_t i = 0; i < n && i < size; ++i )
//			data[i] = va_arg( argptr, T );
//		for ( size_t i = n; i < size; ++i )
//			data[i] = T();
//	}
//*/
//	array(size_t n, T t0) {
//		data[0] = t0;
//		for ( size_t i = 1; i < size; ++i )
//			data[i] = T();
//	}
//	array(size_t n, T t0, T t1) {
//		data[0] = t0;
//		data[1] = t1;
//		for ( size_t i = 2; i < size; ++i )
//			data[i] = T();
//	}
//	array(size_t n, T t0, T t1, T t2) {
//		data[0] = t0;
//		data[1] = t1;
//		data[2] = t2;
//		for ( size_t i = 3; i < size; ++i )
//			data[i] = T();
//	}
//	array(size_t n, T t0, T t1, T t2, T t3) {
//		data[0] = t0;
//		data[1] = t1;
//		data[2] = t2;
//		data[3] = t3;
//		for ( size_t i = 4; i < size; ++i )
//			data[i] = T();
//	}
//	array(size_t n, T t0, T t1, T t2, T t3, T t4) {
//		data[0] = t0;
//		data[1] = t1;
//		data[2] = t2;
//		data[3] = t3;
//		data[4] = t4;
//		for ( size_t i = 5; i < size; ++i )
//			data[i] = T();
//	}
//	array(size_t n, T t0, T t1, T t2, T t3, T t4, T t5) {
//		data[0] = t0;
//		data[1] = t1;
//		data[2] = t2;
//		data[3] = t3;
//		data[4] = t4;
//		data[5] = t5;
//		for ( size_t i = 6; i < size; ++i )
//			data[i] = T();
//	}
//  array(size_t n, T t0, T t1, T t2, T t3, T t4, T t5, T t6) {
//		data[0] = t0;
//		data[1] = t1;
//		data[2] = t2;
//		data[3] = t3;
//		data[4] = t4;
//		data[5] = t5;
//		data[6] = t6;
//		for ( size_t i = 7; i < size; ++i )
//			data[i] = T();
//	}
//	~array() {}
//	T & operator[]( unsigned int index ) {
//		return data[index];
//	}
//	const T & operator[]( unsigned int index ) const {
//		return data[index];
//	}
//
//	const bool operator==(const Niflib::array<size, T> & other) const {
//		for (size_t i = 0; i < size; ++i)
//			if (!(data[i] == other.data[i]))
//				return false;
//		return true;
//	}
//
//private:
//	T data[size];
//};

//TODO:  This is temporary to make it compile.  Should eventually be adjusted to display 1's and 0's insted of as an int.
typedef unsigned short Flags;
}
#endif
