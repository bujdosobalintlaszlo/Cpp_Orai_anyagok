#include<utility>
#include<cassert>
#include<iostream>
#include<ostream>
int gcd(int a,int b){
	 while(b){
		  std::swap(a,b);
		  b%=a;
	 }
	 return a;
}

int gcd_rec(int a,int b){
	 if(b==0){
		  return a;
	 }
	 gcd_rec(a%b,a);
	 
}

struct rational{
	 int nom,denom;
	 rational(int m,int d) : nom(m), denom(d) {
		  assert(d && "d must be non-zero");
		  assert(gcd(m,d) == 1 && "m and d must be co-prime");
	 }

	 rational operator*(rational r1,rational r2){
		  const int nom = r1.nom * r2.nom;
		  const int denom = r1.denom * r2.denom;
		  int gcd_nom= gcd(nom,denom);
		  return rational(nom/gcd_nom,denom/gcd_denom);
	 }

	 bool operator==(rational r1,rational r2){
		  return r1.nom == r2.nom && r1.denom == r2.denom;
	 }

	 std::ostream& operator<<(std::ostream& str,rational r){
		  str << r.nom << '/' << r.denom;
		 return str; 
	 }

};
template<int a,int b>
struct gcd_tmp {
	 static const int value=gcd_tmp<b, a%b>::value;
};
template<int a>
struct gcd_tmp<a,0>{
	 static const int value =a;
};
template<int n,int d>
struct rational_tmp{
	 static_assert(gcd_tmp<n,d>::value == 1,"n and d must be co-primes");
	 static const int nom = n;
	 static const int denom = d;

};
template<typename r1,typename r2>
struct mult{
	 static const int gcd=gcd_tmp<r1::nom*r2::nom,r1::denom * r2::denom>::value;
	 using type = rational_tmp<r1::nom*r2::nom,r1::denom * r2::denom>;
};
 template<T>
 struct isSame{
	 static const bool value = true;
 };

int main(){
 //std::cout << rational(2,3) * rational(3,2) << '\n';
	 return mult <rational_tmp<9,7>,rational_tmp<8,6>>::type;
}
