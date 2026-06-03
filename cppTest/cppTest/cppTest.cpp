// cppTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <functional>
#include "araccpart.h"
#include <algorithm>
#include <string>
#include "araccpart.h"

struct is_even
{

    bool operator()(int i) const
    {
        return 0 == i % 2;
    }

};

struct string_pred
{

    bool operator()(const std::string& s) const
    {
        return s.find("Hell") != std::string::npos;
    }

};

int main()
{
    int your_mark = 1;
    /* 2-es*/
    int a[] = { 7, 3, 4, 1, 2 };
    array_accumulate_partition<int> ap( a, sizeof( a ) / sizeof( a[ 0 ] ) );
    ap.set_partition_idx( 2 );
    ap.set_partition_idx( 4 );
    ap.set_partition_idx( 1 );

    double d[] = { 1.5, 1.8, 3.8, 1.1 };
    array_accumulate_partition<double> dp( d, sizeof( d ) / sizeof( d[ 0 ] ) );
    dp.set_partition_idx( 3 );

    std::string s[] = { "Hello", "World", "!" };
    array_accumulate_partition<std::string> sp( s,
                                                sizeof( s ) / sizeof( s[ 0 ] ) );
    sp.set_partition_idx( 2 );
    sp.set_partition_idx( 1 );
    sp.set_partition_idx( 2 );

    if( "!" == s[ 2 ] && 3.14 < d[ 1 ] && 7.0 < d[ 2 ] && 3 == a[ 1 ] &&
        4 == a[ 2 ] && 1.2 > d[ 3 ] && 1.55 > d[ 0 ] && "Hello" == s[ 0 ] )
    {
      your_mark = std::count( s[ 1 ].begin(), s[ 1 ].end(), 'o' );
    }
    /**/
    /* 3-as*/
    sp >> 1;
    2 << dp;
    3 >> ap;
    if( 15 == a[ 3 ] && a[ 1 ] * 1U == s[ 1 ].size() && 1.9 > d[ 1 ] )
    {
      your_mark = std::count( s[ 2 ].begin(), s[ 2 ].end(), 'l' );
    }
   /* */
    /* 4-es*/
    if( 3 == std::count_if( sp.abegin(), sp.aend(), string_pred() ) &&
        *ap.begin() == std::count_if( ap.abegin(), ap.aend(), is_even() ) )
    {
      your_mark = ap.aend() - ap.abegin();
    }
   /* */
    /* 5-os */
    int m[] = { 5, 3, 2, 1, 6 };
    int t[] = { 2, 2, 2, 2, 2, 2 };
    array_accumulate_partition<int, std::multiplies<int> > mp( m );
    array_accumulate_partition<int, std::multiplies<int> > tp( t );
    tp.set_partition_idx( 4 );
    mp.set_partition_idx( 2 );
    mp >> 1;
    if( 30 == m[ 2 ] && 1 == m[ 3 ] && 16 == t[ 3 ] && 2 == t[ 4 ] &&
        1 == *mp.begin() && 15 == m[ 1 ] )
    {
      your_mark = ( mp.end() - mp.begin() ) + ( mp.aend() - mp.abegin() );
    }
    /**/
    std::cout << "Your mark is " << your_mark;
    std::endl(std::cout);
}

