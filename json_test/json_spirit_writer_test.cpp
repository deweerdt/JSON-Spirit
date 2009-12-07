//          Copyright John W. Wilkinson 2007 - 2009.
// Distributed under the MIT License, see accompanying file LICENSE.txt

// json spirit version 4.02

#include "json_spirit_writer_test.h"
#include "json_spirit_writer.h"
#include "json_spirit_value.h" 
#include "utils_test.h"

#include <sstream>
#include <boost/integer_traits.hpp>

using namespace json_spirit;
using namespace std;
using namespace boost;

namespace
{
    const int64_t max_int64   = integer_traits< int64_t  >::max();
    const uint64_t max_uint64 = integer_traits< uint64_t >::max();

    template< class Config_type >
    struct Test_runner
    {
        typedef typename Config_type::String_type String_type;
        typedef typename Config_type::Object_type Object_type;
        typedef typename Config_type::Array_type Array_type;
        typedef typename Config_type::Value_type Value_type;
        typedef typename String_type::value_type Char_type;
        typedef typename String_type::const_iterator Iter_type;
        typedef std::basic_ostream< Char_type > Ostream_type;

        String_type to_str( const char* c_str )
        {
            return ::to_str< String_type >( c_str );
        }
 
        void add_value( Object_type& obj, const char* c_name, const Value_type& value )
        {
            Config_type::add( obj, to_str( c_name ), value );
        }

        void add_c_str( Object_type& obj, const char* c_name, const char* c_value )
        {
            add_value( obj, c_name, to_str( c_value ) );
        }

        void check_eq( const Value_type& value, const char* expected_result )
        {
            assert_eq( write( value ), to_str( expected_result ) );
        }

        void check_eq_pretty( const Value_type& value, const char* expected_result )
        {
            assert_eq( write_formatted( value ), to_str( expected_result ) );
        }

        void test_empty_obj()
        {
            check_eq( Object_type(), "{}" );
            check_eq_pretty( Object_type(), "{\n"
                                         "}" );
        }

        void test_obj_with_one_member()
        {
            Object_type obj;

            add_c_str( obj, "name", "value" );

            check_eq       ( obj, "{\"name\":\"value\"}" );
            check_eq_pretty( obj, "{\n"
                                  "    \"name\" : \"value\"\n"
                                  "}" );
        }

        void test_obj_with_two_members()
        {
            Object_type obj;

            add_c_str( obj, "name_1", "value_1" );
            add_c_str( obj, "name_2", "value_2" );

            check_eq( obj, "{\"name_1\":\"value_1\",\"name_2\":\"value_2\"}" );

            check_eq_pretty( obj, "{\n"
                                  "    \"name_1\" : \"value_1\",\n"
                                  "    \"name_2\" : \"value_2\"\n"
                                  "}" );
        }

        void test_obj_with_three_members()
        {
            Object_type obj;

            add_c_str( obj, "name_1", "value_1" );
            add_c_str( obj, "name_2", "value_2" );
            add_c_str( obj, "name_3", "value_3" );

            check_eq( obj, "{\"name_1\":\"value_1\",\"name_2\":\"value_2\",\"name_3\":\"value_3\"}" );

            check_eq_pretty( obj, "{\n"
                                  "    \"name_1\" : \"value_1\",\n"
                                  "    \"name_2\" : \"value_2\",\n"
                                  "    \"name_3\" : \"value_3\"\n"
                                  "}" );
        }

        void test_obj_with_one_empty_child_obj()
        {
            Object_type child;

            Object_type root;

            add_value( root, "child", child );

            check_eq( root, "{\"child\":{}}" );

            check_eq_pretty( root, "{\n"
                                   "    \"child\" : {\n"
                                   "    }\n"
                                   "}" );
        }

        void test_obj_with_one_child_obj()
        {
            Object_type child;

            add_c_str( child, "name_2", "value_2" );

            Object_type root;

            add_value( root, "child", child );
            add_c_str( root, "name_1", "value_1" );

            check_eq( root, "{\"child\":{\"name_2\":\"value_2\"},\"name_1\":\"value_1\"}" );

            check_eq_pretty( root, "{\n"
                                   "    \"child\" : {\n"
                                   "        \"name_2\" : \"value_2\"\n"
                                   "    },\n"
                                   "    \"name_1\" : \"value_1\"\n"
                                   "}" );
        }

        void test_obj_with_grandchild_obj()
        {
            Object_type child_1; add_c_str( child_1, "name_1", "value_1" );
            Object_type child_2; 
            Object_type child_3; add_c_str( child_3, "name_3", "value_3" );

            add_value( child_2, "grandchild", child_3 );
            add_c_str( child_2, "name_2", "value_2" );

            Object_type root;

            add_value( root, "child_1", child_1 );
            add_value( root, "child_2", child_2 );
            add_c_str( root, "name_a", "value_a" );
            add_c_str( root, "name_b", "value_b" );

            check_eq( root, "{\"child_1\":{\"name_1\":\"value_1\"},"
                            "\"child_2\":{\"grandchild\":{\"name_3\":\"value_3\"},\"name_2\":\"value_2\"},"
                            "\"name_a\":\"value_a\","
                            "\"name_b\":\"value_b\"}" );

            check_eq_pretty( root, "{\n"
                                   "    \"child_1\" : {\n"
                                   "        \"name_1\" : \"value_1\"\n"
                                   "    },\n"
                                   "    \"child_2\" : {\n"
                                   "        \"grandchild\" : {\n"
                                   "            \"name_3\" : \"value_3\"\n"
                                   "        },\n"
                                   "        \"name_2\" : \"value_2\"\n"
                                   "    },\n"
                                   "    \"name_a\" : \"value_a\",\n"
                                   "    \"name_b\" : \"value_b\"\n"
                                   "}" );
        }

        void test_objs_with_bool_pairs()
        {
            Object_type obj;

            add_value( obj, "name_1", true  );
            add_value( obj, "name_2", false );
            add_value( obj, "name_3", true  );
     
            check_eq( obj, "{\"name_1\":true,\"name_2\":false,\"name_3\":true}" );
        }

        void test_objs_with_int_pairs()
        {
            Object_type obj;

            add_value( obj, "name_1", 11 );
            add_value( obj, "name_2", INT_MAX );
            add_value( obj, "name_3", max_int64 );

            ostringstream os;

            os << "{\"name_1\":11,\"name_2\":" << INT_MAX << ",\"name_3\":" << max_int64 << "}";
     
            check_eq( obj, os.str().c_str() );
        }

        void test_objs_with_real_pairs()
        {
            Object_type obj;

            add_value( obj, "name_1", 1.0 );
            add_value( obj, "name_2", 1.234567890123456e-108 );
            add_value( obj, "name_3", -1234567890.123456 );
            add_value( obj, "name_4", -1.2e-126 );
     
            check_eq( obj, "{\"name_1\":1.000000000000000,"
                           "\"name_2\":1.234567890123456e-108,"
                           "\"name_3\":-1234567890.123456,"
                           "\"name_4\":-1.200000000000000e-126}" );
        }

        void test_objs_with_null_pairs()
        {
            Object_type obj;

            add_value( obj, "name_1", Value_type::null );
            add_value( obj, "name_2", Value_type::null );
            add_value( obj, "name_3", Value_type::null );
     
            check_eq( obj, "{\"name_1\":null,\"name_2\":null,\"name_3\":null}" );
        }

        void test_empty_array()
        {
            check_eq( Array_type(), "[]" );
            check_eq_pretty( Array_type(), "[\n"
                                        "]" );
        }

        void test_array_with_one_member()
        {
            Array_type arr;

            arr.push_back( to_str( "value" ) );

            check_eq       ( arr, "[\"value\"]" );
            check_eq_pretty( arr, "[\n"
                                  "    \"value\"\n"
                                  "]" );
        }

        void test_array_with_two_members()
        {
            Array_type arr;

            arr.push_back( to_str( "value_1" ) );
            arr.push_back( 1 );

            check_eq       ( arr, "[\"value_1\",1]" );
            check_eq_pretty( arr, "[\n"
                                  "    \"value_1\",\n"
                                  "    1\n"
                                  "]" );
        }

        void test_array_with_n_members()
        {
            Array_type arr;

            arr.push_back( to_str( "value_1" ) );
            arr.push_back( 123 );
            arr.push_back( 123.456 );
            arr.push_back( true );
            arr.push_back( false );
            arr.push_back( Value_type() );

            check_eq       ( arr, "[\"value_1\",123,123.4560000000000,true,false,null]" );
            check_eq_pretty( arr, "[\n"
                                  "    \"value_1\",\n"
                                  "    123,\n"
                                  "    123.4560000000000,\n" 
                                  "    true,\n"
                                  "    false,\n"
                                  "    null\n"
                                  "]" );
        }

        void test_array_with_one_empty_child_array()
        {
            Array_type arr;

            arr.push_back( Array_type() );

            check_eq       ( arr, "[[]]" );
            check_eq_pretty( arr, "[\n"
                                  "    [\n"
                                  "    ]\n"
                                  "]" );
        }

        void test_array_with_one_child_array()
        {
            Array_type child;

            child.push_back( 2 );

            Array_type root;

            root.push_back( 1 );
            root.push_back( child );

            check_eq       ( root, "[1,[2]]" );
            check_eq_pretty( root, "[\n"
                                   "    1,\n"
                                   "    [\n"
                                   "        2\n"
                                   "    ]\n"
                                   "]" );
        }

        void test_array_with_grandchild_array()
        {
            Array_type child_1; child_1.push_back( 11 );
            Array_type child_2; child_2.push_back( 22 );
            Array_type child_3; child_3.push_back( 33 );

            child_2.push_back( child_3 );

            Array_type root;

            root.push_back( 1);
            root.push_back( child_1 );
            root.push_back( child_2 );
            root.push_back( 2 );

            check_eq       ( root, "[1,[11],[22,[33]],2]" );
            check_eq_pretty( root, "[\n"
                                   "    1,\n"
                                   "    [\n"
                                   "        11\n"
                                   "    ],\n"
                                   "    [\n"
                                   "        22,\n"
                                   "        [\n"
                                   "            33\n"
                                   "        ]\n"
                                   "    ],\n"
                                   "    2\n"
                                   "]" );
        }

        void test_array_and_objs()
        {
            Array_type a;

            a.push_back( 11 );

            Object_type obj;

            add_value( obj, "a", 1 );

            a.push_back( obj );

            check_eq       ( a, "[11,{\"a\":1}]" );
            check_eq_pretty( a, "[\n"
                                "    11,\n"
                                "    {\n"
                                "        \"a\" : 1\n"
                                "    }\n"
                                "]" );

            add_value( obj, "b", 2 );

            a.push_back( 22 );
            a.push_back( 33 );
            a.push_back( obj );

            check_eq       ( a, "[11,{\"a\":1},22,33,{\"a\":1,\"b\":2}]" );
            check_eq_pretty( a, "[\n"
                                "    11,\n"
                                "    {\n"
                                "        \"a\" : 1\n"
                                "    },\n"
                                "    22,\n"
                                "    33,\n"
                                "    {\n"
                                "        \"a\" : 1,\n"
                                "        \"b\" : 2\n"
                                "    }\n"
                                "]" );
        }

        void test_obj_and_arrays()
        {
            Object_type obj;

            add_value( obj, "a", 1 );

            Array_type a;

            a.push_back( 11 );

            add_value( obj, "b", a );

            check_eq       ( obj, "{\"a\":1,\"b\":[11]}" );
            check_eq_pretty( obj, "{\n"
                                  "    \"a\" : 1,\n"
                                  "    \"b\" : [\n"
                                  "        11\n"
                                  "    ]\n"
                                  "}" );

            a.push_back( obj );

            add_value( obj, "c", a );

            check_eq       ( obj, "{\"a\":1,\"b\":[11],\"c\":[11,{\"a\":1,\"b\":[11]}]}" );
            check_eq_pretty( obj, "{\n"
                                  "    \"a\" : 1,\n"
                                  "    \"b\" : [\n"
                                  "        11\n"
                                  "    ],\n"
                                  "    \"c\" : [\n"
                                  "        11,\n"
                                  "        {\n"
                                  "            \"a\" : 1,\n"
                                  "            \"b\" : [\n"
                                  "                11\n"
                                  "            ]\n"
                                  "        }\n"
                                  "    ]\n"
                                  "}" );
        }

        void test_escape_char( const char* esc_str_in, const char* esc_str_out )
        {
            Object_type obj;

            const string name_str( string( esc_str_in ) + "name" );

            add_value( obj, name_str.c_str(), to_str( "value" ) + to_str( esc_str_in ) );

            const string out_str( string( "{\"" ) + esc_str_out + "name\":\"value" + esc_str_out + "\"}" );

            check_eq( obj, out_str.c_str() );
        }

        void test_escape_chars()
        {
            test_escape_char( "\r", "\\r" );
            test_escape_char( "\n", "\\n" );
            test_escape_char( "\t", "\\t" );
            test_escape_char( "\f", "\\f" );
            test_escape_char( "\b", "\\b" );
            test_escape_char( "\"", "\\\"" );
            test_escape_char( "\\", "\\\\" );
            test_escape_char( "\x01", "\\u0001" );
            test_escape_char( "\x12", "\\u0012" );
            test_escape_char( "\x7F", "\\u007F" );
        }

        void test_to_stream()
        {
            basic_ostringstream< Char_type > os;

            Array_type arr;

            arr.push_back( 1 );
            arr.push_back( 2 );

            write( arr, os );

            assert_eq( os.str(), to_str( "[1,2]" ) );
        }

        void test_values()
        {
            check_eq( 123, "123" );
            check_eq( 1.234, "1.234000000000000" );
            check_eq( to_str( "abc" ), "\"abc\"" );
            check_eq( false, "false" );
            check_eq( Value_type::null, "null" );
        }

        void test_uint64()
        {
            check_eq( Value_type( 0 ),             "0" );
            check_eq( Value_type( int64_t( 0 ) ),  "0" );
            check_eq( Value_type( uint64_t( 0 ) ), "0" );

            check_eq( Value_type( 1 ),             "1" );
            check_eq( Value_type( int64_t( 1 ) ),  "1" );
            check_eq( Value_type( uint64_t( 1 ) ), "1" );

            check_eq( Value_type( -1 ),            "-1" );
            check_eq( Value_type( int64_t( -1 ) ), "-1" );

            check_eq( Value_type( max_int64 ),             "9223372036854775807" );
            check_eq( Value_type( uint64_t( max_int64 ) ), "9223372036854775807" );

            check_eq( Value_type( max_uint64 ), "18446744073709551615" );
        }

        void run_tests()
        {
            test_empty_obj();
            test_obj_with_one_member();
            test_obj_with_two_members();
            test_obj_with_three_members();
            test_obj_with_one_empty_child_obj();
            test_obj_with_one_child_obj();
            test_obj_with_grandchild_obj();
            test_objs_with_bool_pairs();
            test_objs_with_int_pairs();
            test_objs_with_real_pairs();
            test_objs_with_null_pairs();
            test_empty_array();
            test_array_with_one_member();
            test_array_with_two_members();
            test_array_with_n_members();
            test_array_with_one_empty_child_array();
            test_array_with_one_child_array();
            test_array_with_grandchild_array();
            test_array_and_objs();
            test_obj_and_arrays();
            test_escape_chars();
            test_to_stream();
            test_values();
            test_uint64();
        }
    };

#ifndef BOOST_NO_STD_WSTRING
    void test_wide_esc_u( wchar_t c, const wstring& result)
    {
        const wstring s( 1, c );

        wArray arr( 1, s );

        assert_eq( write( arr ), L"[\"\\u" + result + L"\"]" );
    }

    void test_wide_esc_u()
    {
        test_wide_esc_u( 0xABCD, L"ABCD" );
        test_wide_esc_u( 0xFFFF, L"FFFF" );
    }
#endif

    bool is_printable( char c )
    {
        const wint_t unsigned_c( ( c >= 0 ) ? c : 256 + c );

        return iswprint( unsigned_c ) != 0;
    }

    void test_extended_ascii()
    {
        const string expeced_result( is_printable( '�' ) ? "[\"����\"]" : "[\"\\u00E4\\u00F6\\u00FC\\u00DF\"]" );

        assert_eq( write( Array( 1, "����" ) ), expeced_result );
    }
}

void json_spirit::test_writer()
{
    Test_runner< Config  >().run_tests();
    Test_runner< mConfig >().run_tests();

#ifndef BOOST_NO_STD_WSTRING
    Test_runner< wConfig  >().run_tests();
    Test_runner< wmConfig >().run_tests();
    test_wide_esc_u();
#endif

    test_extended_ascii();
}
