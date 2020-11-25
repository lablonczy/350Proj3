#include <iostream>
#include <vector>

using namespace std;

vector<int> karatsuba(vector<int> &a, vector<int> &b);
int getBiggestLength(vector<int> &a, vector<int> &b);
void prepend(vector<int> &a, int &finalSize);
void printvector(vector<int> &a);
void shiftUp(vector<int> &output);
vector<int> getLastN(vector<int> &a, int &n);
vector<int> add(vector<int> &a, vector<int> &b);
vector<int> subtract(vector<int> &a, vector<int> &b);
void flipAll(vector<int> &a);
vector<int> append(vector<int> &list, const int &exponent);
void strikeLeadingZeroes(vector<int> &product);
void karatsubaOuter(vector<int> a, vector<int> b);

void penPaperMult(vector<int> &a, vector<int> &b);
vector<int> traditionalMult(vector<int> &multiplicand, vector<int> &multiplier);

void debug(string s);

int main() {
    vector<int> a, b;
    
    string s;
    cin >> s;
    unsigned int i;

    for (i = 0; s[i] != '*'; i++)
        a.push_back(s[i] - '0');  
    
    for (i++; i<s.size(); i++)
        b.push_back(s[i] - '0');
	    
    cout << "** Brute Force: ";
    penPaperMult(a,b);
    cout << "** Karatsuba: ";
    karatsubaOuter(a,b);
    return 0;
}


void debug(string s){
	cout << s << endl;
}

void penPaperMult(vector<int> &a, vector<int> &b){
    vector<int> result = traditionalMult(a,b);
    strikeLeadingZeroes(result);
    printvector(result);
}

vector<int> traditionalMult(vector<int> &multiplicand, vector<int> &multiplier){
    vector<int> outputArray;
    outputArray.resize(multiplicand.size() + multiplier.size());
		for(unsigned int i=0;i<outputArray.size();i++) 
			outputArray[i] = 0;
		
		for(int j=multiplier.size()-1;j>=0;j--)
			for(int i=multiplicand.size() - 1;i>=0;i--) {
				int subResult = multiplicand[i] * multiplier[j];
				
				int onesPlace = subResult%10;
				int tensPlace = subResult/10;
				
				outputArray[i+1+j] += onesPlace;
				outputArray[i+j] += tensPlace;
			}
		
		shiftUp(outputArray);
		
		return outputArray;
		
}

void karatsubaOuter(vector<int> a, vector<int> b){
    vector<int> karatsubaNum = karatsuba(a,b);
    strikeLeadingZeroes(karatsubaNum);
    printvector(karatsubaNum);
}

void strikeLeadingZeroes(vector<int> &product){
    for(unsigned int i=0;i<product.size();i++) {
		if(product[i] != 0 || product.size() == 1)
			return;
		
		product.erase(product.begin());
		i--;
	}
}

vector<int> karatsuba(vector<int> &a, vector<int> &b){
    int big = getBiggestLength(a,b);
    prepend(a, big);
    prepend(b, big);
    
    if(a.size() == 1) {
			vector<int> result;
			result.push_back(a.front() * b.front());
			shiftUp(result);
			return result;
		}
    
    int m = big/2;
	vector<int> tempa = a;
	vector<int> tempb = b;
	vector<int> a0 = getLastN(tempa,m), a1 = tempa, b0 = getLastN(tempb,m), b1 = tempb;
	
	if(a1.size()==0)
		a1.push_back(0);
	if(b1.size()==0)
		b1.push_back(0);
		
	vector<int> c2 = karatsuba(a1,b1);
	vector<int> c0 = karatsuba(a0,b0);
	
	vector<int> test = add(a1,a0);
	vector<int> test1 = add(b1,b0);
	vector<int> test2 = karatsuba(test,test1);
	
	vector<int> test3 = subtract(test2,c2);
	vector<int> test4 = subtract(test3,c0);
	vector<int> c1 = test4;
	
	vector<int> returnC2 = append(c2,2*m);
	vector<int> returnC1 = append(c1,m);
	vector<int> returnC0 = c0;
	
	int largestSize = max(max(returnC2.size(), returnC1.size()),returnC0.size());
	
	prepend(returnC2, largestSize);
	prepend(returnC1, largestSize);
	prepend(returnC0, largestSize);
	
	vector<int> test5 = add(returnC1,returnC0);
	vector<int> test6 = add(returnC2,test5);
	return test6;
}

vector<int> getLastN(vector<int> &a, int &n){
    vector<int> temp;
		for(int i=0;i<n;i++){
		    int back = a.back();
		    a.pop_back();
			temp.insert(temp.begin(),back);
		}
			
	return temp;
}

vector<int> add(vector<int> &a, vector<int> &b){
    vector<int> tempa = a, tempb = b;
	int largerSize = getBiggestLength(a,b);
	prepend(tempa, largerSize);
	prepend(tempb, largerSize);
	
	vector<int> result;
	for(unsigned int i=0;i<a.size();i++){
	    int aBack = tempa.back(), bBack = tempb.back();
	    tempa.pop_back();
	    tempb.pop_back();
		result.insert(result.begin(),aBack + bBack);
	}
	
	shiftUp(result);
	
	return result;
}

void flipAll(vector<int> &a){
    for(unsigned int i=0;i<a.size();i++)
		a[i] = -1 * a[i];
}

vector<int> subtract(vector<int> &a, vector<int> &b){
    vector<int> tempa = a, tempb = b;
	flipAll(tempb);
	int largerSize = getBiggestLength(a,b);
	prepend(tempa, largerSize);
	prepend(tempb, largerSize);
	vector<int> result = add(tempa,tempb);
	
	for(unsigned int i=0;i<result.size()-1;i++) {
		int current = result[i], next = result[i+1];
		if(next < 0) {
			result[i] = current-1;
			result[i+1] = 10+next;
			if(result[i] < 0)
				i-=2;
		}
	}
		
	return result;
}

vector<int> append(vector<int> &list, const int &exponent){
    vector<int> tempa = list;
	for(int i=0;i<exponent;i++)
		tempa.push_back(0);
	
	return tempa;
}

void shiftUp(vector<int> &output) {
	int temp(0);
	for(int i=output.size() - 1;i>=0;i--) {
		temp = output[i];
		if(temp >= 10) {
			output[i] = temp%10;
			if(i-1 < 0) {
				output.insert(output.begin(),temp/10);
			} else {
				output[i-1] = output[i-1] + temp/10;
			}
		}
	}
}

void printvector(vector<int> &a){
    for(const int &i : a)
        cout << i;
    cout << endl;
}

int getBiggestLength(vector<int> &a, vector<int> &b){
    return std::max(a.size(), b.size());
}

void prepend(vector<int> &a, int &finalSize){
    int size = a.size();
    if(size == finalSize)
        return;
    
    int numZeroes = finalSize - size;
    for(int i=0;i<numZeroes;i++)
        a.insert(a.begin(),0);
}
    


