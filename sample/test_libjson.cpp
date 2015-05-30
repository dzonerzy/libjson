#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cstdio>
#include <random>   
#include <cstring>
#include <sstream>
#include <vector>
#include "libjson.h"
using namespace std;

template<typename T>
std::string toString(T val ) { // not terribly efficient, but works...
   std::ostringstream myostringstream;
   myostringstream << val;
   return myostringstream.str();
}

long get_file_size( char const*filepath ) {
    FILE *file = fopen(filepath, "r");
    if( file == 0 ) {
        throw runtime_error( string("File ") + filepath + " couldn't be opened" );
    }

    long len = 0;
    fseek(file, 0L, SEEK_END);  /* Position to end of file */
    len = ftell(file);     /* Get file length */
    fclose( file );
    return len;
}

char *read_file( char const*filepath ) {
    long file_len = get_file_size( filepath );
    char *buffer = new char[ file_len ];
    FILE *file = fopen(filepath, "r");
    int res = fread( buffer, 1, file_len, file );
    fclose( file );
    return buffer;
}

void write_file( char const *filepath, char const *data, int len ) {
    FILE *file = fopen(filepath, "w" );
    fwrite( data, 1, len, file );
    fclose(file);
}

void walk_node( JSONNODE *node ) {
    switch( json_type( node ) ) {
        case 1:
            cout << "string node " << json_as_string( node ) << endl;
            break;
        case 4:
            for( int i = 0; i < json_size( node ); i++ ) {
                JSONNODE *child = json_at( node, i );
                walk_node( child );
            }
            break;
        case 5:
            
        default:
            throw runtime_error( "unhandled type " + toString( (int)json_type( node ) ) );
    }
}

void test1() {
//    const char *json_text = "[\"one\", \"two\", \"three\", {" +
//"    \"name\": \"Simon\"," +
//"    \"age\": 25 " +
//}]";
    char *json = read_file( "../sample/test.json" );
    cout << "json: " << json << endl;
    JSONNODE *node = json_parse( json );
    json_preparse( node );
    cout << node << endl;
    //cout << node.type() << endl;
    cout << json_size( node ) << endl;
    cout << (int)json_type( node ) << endl;
    cout << json_write_formatted( node ) << endl;
    walk_node( node );
    delete[] json;
}

void test2() {
    JSONNODE *node = json_new( 4 );
    int length = 5 * 5 * 32 * 12 + 361 * 28 * 28 * 18;
    mt19937 random;
    random.seed(0);
    float *data = new float[length];
    for( int i = 0; i < length; i++ ) {
        data[i] = ( random() % 10000000 ) / 10000000.0f;
    }
    cout << "length " << length << endl;
    write_file( "test_out.dat", reinterpret_cast< char * >( data ), length * 4 );
    cout << "written data" << endl;
    for( int i = 0; i < length; i++ ) {
        JSONNODE *child = json_new_f( 0, data[i] );
        json_push_back( node, child );
    }
    char *json_str = json_write( node );
//    cout << json_str << endl;
    int len = strlen( json_str );
    write_file( "test_out.json", json_str, len );
    cout << "written json" << endl;
}

int main( int argc, char *argv[] ) {
    // test1();
    test2();
    return 0;
}

