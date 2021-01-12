#ifndef _LSH_FUNCTIONS_H
#define _LSH_FUNCTIONS_H 

#include <vector>
#include <cmath>




template< typename S >
S calculate_h( S *s_arr , vector<S> data , int n , int w , int m , int M ){

    int current_modulo = (m%M)%M;  
    int const_modulo = current_modulo;

    S f_a,s,x,m_exp,n_m;
    S h_val = 0;

    typename vector<S>::reverse_iterator rit = data.rbegin();

    int i = n-1;
    for ( i ; i >= 0; i--  ){
        
        x = data[i];
        s = s_arr[i];
        f_a = floor( double((  double(x) - double(s)) / double(w)));


        // cout<<" a is "<<f_a<<endl;

 
        if ( i == n-1 ){
            h_val += abs((f_a%M)%M);
        }
        else{
            h_val += (abs(f_a%M) * current_modulo )%M;
            current_modulo = (current_modulo * const_modulo) % M ;
        }
    }

    return h_val % M;


}

#endif