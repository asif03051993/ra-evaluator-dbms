#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <vector>
#include <math.h>
#include<sstream>
#include <algorithm> //std::unique

using namespace std;

class Schema{                   //An Object for storing the schema of a Table
public:
	vector <string> attr;       //Attributes
    vector <string> type;       //Types
	Schema(){
		attr.clear();
		type.clear();
	}
};
class Record{                   //An Object for storing a row of elements
public:
	vector <string> element;
	Record(){
		element.clear();
	}
};
class Relation{                  //An Object for storing a Table
public:
	Schema r_schema;
	string name;
	vector <Record> tupple;
	Relation(){}
	Relation(const char* file){         //Reading a relation from a file
		fstream in;
		in.open(file,ios::in);
		if(!in){
			cout<<"Error in opening file"<<endl;
		}
		string str1 = file;//'name.assign(file);
        unsigned pos = str1.find(".");
		name = str1.substr(0,pos);
		string str;str.clear();
		char ch[2];bool type = false,row = false;Record entry;
		while(!in.eof()){
			ch[0] = in.get();
			ch[1] = '\0';
			if(ch[0] == ' ')
				continue;
			if(ch[0] == ':'){
				r_schema.attr.push_back(str.data());
				str.clear();
				type = true;
			}
			else if( (ch[0] == '|'|| ch[0] == '\n') && type ){
				if(ch[0] == '\n')
					row = true;
				r_schema.type.push_back(str.data());
				str.clear();
				type = false;
			}
			else if(ch[0] == '|' && row ){
				entry.element.push_back(str.data());
				str.clear();
			}
			else if(ch[0] == '\n' && row){
				entry.element.push_back(str.data());
				tupple.push_back(entry);
				str.clear();
				entry.element.clear();
			}
			else
				str = str + ch;
		}
		remove_dup();
	}
	void remove_dup(){                      //A Function remove duplicates from a table
		vector <Record>::iterator it1;
		vector <Record>::iterator it2;
		for(it1 = tupple.begin(); it1 != tupple.end(); it1++){
			for(it2 = it1; it2 != tupple.end(); it2++){
				if(it2 == it1)
					continue;
				int p = 0;
				for(int i=0; i<it2->element.size(); i++){
					if(it2->element[i].compare(it1->element[i]) == 0)
						p++;
				}
				if(p == it2->element.size()){
					tupple.erase(it2);
					it2--;
				}
			}
		}
	}
	void printtupples(){                    //A Function to print a Table
	    cout<<"Relation :"<<name<<endl;
		for(int i=0; i<r_schema.attr.size(); i++){
			cout<<setw(15)<<r_schema.attr[i]<<":"<<r_schema.type[i];
			if(i != r_schema.attr.size()-1)
				cout<<" | ";
		}
		cout<<endl;
		for(int i=0; i<tupple.size(); i++){
			for(int j=0; j<tupple[i].element.size(); j++){
				cout<<setw(15)<<tupple[i].element[j];
				if(j != tupple[i].element.size()-1)
					cout<<" | ";
			}
			cout<<endl;
		}
	}
	Record* getnextrecord(int k){
		if(k+1 > tupple.size())
			return NULL;
		return &tupple[k];
	}
};
void writetofile(Relation r,const char*file){       //Write a table to a file
	fstream out(file,ios::out);
	for(int i=0; i < r.r_schema.attr.size(); i++){
		out<<setw(15)<<r.r_schema.attr[i]<<":"<<r.r_schema.type[i];
		if(i != r.r_schema.attr.size()-1)
			out<<" | ";
	}
	out<<endl;
	for(int i=0; i < r.tupple.size(); i++){
		for(int j=0; j < r.tupple[i].element.size(); j++){
			out<<setw(15)<<r.tupple[i].element[j];
			if(j != r.tupple[i].element.size()-1)
				out<<" | ";
		}
		out<<endl;
	}
}
template <class mytype>
class list{
public:
	vector<mytype> array;
	bool addtolist(mytype in){
		array.push_back(in);
		return true;
	}
};
//functions for union between schemas
class operators{
};

class unaryoperators : public operators {       //A class for maintaining  functions related to Unary aoperators
public:
int findindex(string str, Schema s1)       //A Function to find the index of a certain attribute in a table
{
     for(int i = 0; i < s1.attr.size(); i++){
        if(s1.attr[i] == str){
            if(s1.type[i]  != "string")
                return i;
            else{
            cout<<"Error:No arithmetic operations on strings"<<endl;
            exit(0);}
        }
    }
    cout<<"Error: No such atrribute"<<endl;
    exit(0);
}

float compute(string str,Schema s1, Record t1)  //A function to parse and compute an arithmetic operations betwn attributes
{
  float value = 0;
  int j;
  size_t nxtfound = 0;
  size_t found = str.find_first_of("+-(*/");
  if(str[0] != '(')
  {
            nxtfound=str.find_first_of("+-*/");
            if(nxtfound!=string::npos)
                value = atof(t1.element[findindex(str.substr(0,nxtfound),s1)].data());
            else
                value = atof(t1.element[findindex(str.substr(0,str.size()),s1)].data());}
  while (found!=string::npos)
  {
    if(str[found] == '+') //plus
    {
        found++;
        if(str[found] == '('){
                        int k =0;
                        for(j = found+1 ; j<str.size();j++)
                        {
                                        if(str[j] == '(')
                                           k++;
                                        else    if(str[j] == ')' && k!=0)
                                           k--;
                                        else   if(str[j] == ')' && k==0)
                                            break;
                        }
                        value = value + compute(str.substr(found+1,j-found-1),s1,t1);
                        found = j;
        }
        else{
            nxtfound=str.find_first_of("+-()*/",found+1);
            if(nxtfound!=string::npos)
                value = value + atof(t1.element[findindex(str.substr(found,nxtfound-found-1),s1)].data());
            else
                value = value + atof(t1.element[findindex(str.substr(found,str.size()-found),s1)].data());
        }
    }
    else    if(str[found] == '-') //minus
    {
        found++;
        if(str[found] == '('){
                        int k =0;
                        for(j = found+1 ; j<str.size();j++)
                        {
                                        if(str[j] == '(')
                                           k++;
                                        else    if(str[j] == ')' && k!=0)
                                           k--;
                                        else   if(str[j] == ')' && k==0)
                                            break;
                        }
                        value = value - compute(str.substr(found+1,j-found-1),s1,t1);
                        found = j;
        }
        else{
            nxtfound=str.find_first_of("+-()*/",found+1);
            if(nxtfound!=string::npos)
                value = value - atof(t1.element[findindex(str.substr(found,nxtfound-found-1),s1)].data());
            else
                value = value - atof(t1.element[findindex(str.substr(found,str.size()-found),s1)].data());}
    }
    else    if(str[found] == '*') //product
    {
        found++;
        if(str[found] == '('){
                        int k =0;
                        for(j = found+1 ; j<str.size();j++)
                        {
                                        if(str[j] == '(')
                                           k++;
                                        else    if(str[j] == ')' && k!=0)
                                           k--;
                                        else   if(str[j] == ')' && k==0)
                                            break;
                        }
                        value = value * compute(str.substr(found+1,j-found-1),s1,t1);
                        found = j;
        }
        else{
            nxtfound=str.find_first_of("+-()*/",found+1);
            if(nxtfound!=string::npos)
                value = value * atof(t1.element[findindex(str.substr(found,nxtfound-found-1),s1)].data());
            else
                value = value * atof(t1.element[findindex(str.substr(found,str.size()-found),s1)].data());}
    }
    else    if(str[found] == '/') //division
    {
        found++;
        if(str[found] == '('){
                        int k =0;
                        for(j = found+1 ; j<str.size();j++)
                        {
                                        if(str[j] == '(')
                                           k++;
                                        else    if(str[j] == ')' && k!=0)
                                           k--;
                                        else   if(str[j] == ')' && k==0)
                                            break;
                        }
                            value = value / compute(str.substr(found+1,j-found-1),s1,t1);
                        found = j;
        }
        else{
            nxtfound=str.find_first_of("+-()*/",found+1);
            if(nxtfound!=string::npos)
            {
                float temp1 = atof(t1.element[findindex(str.substr(found,nxtfound-found-1),s1)].data());
                    value = value / temp1;
            }
            else
            {
                float temp1 = atof(t1.element[findindex(str.substr(found,str.size()-found),s1)].data());
                    value = value / temp1;
            }
        }
    }
    else    if(str[found] == '(')
    {
        int k =0;
                        for(j = found+1 ; j<str.size();j++)
                        {
                                        if(str[j] == '(')
                                           k++;
                                        else    if(str[j] == ')' && k!=0)
                                           k--;
                                        else   if(str[j] == ')' && k==0)
                                            break;
                        }
                        value = compute(str.substr(found+1,j-found-1),s1,t1);
                        found = j;
    }
    if(found+1 < str.size())
        found=str.find_first_of("+-(*/",found+1);
    else
        break;
  }
  return value;
}

bool IsAnyArith(string lhs) //Is any arithmetic operation in the term
{
        if(lhs.find_first_of("+-(*/") != string::npos)
            return true;
        return false;
}
};

class binaryoperators : public operators{   //A class for maintaining  functions related to Binary aoperators
public:
    bool checkarity(Relation r1, Relation r2)       //Check whether the arity of the relations are same
    {
	if(r1.r_schema.type.size() != r2.r_schema.type.size()){
		cout<<"The Arity's do not match for the schemas"<<endl;
		return false;
	}
        for(int i = 0; i < r1.r_schema.type.size(); i++)
        {
            if(r1.r_schema.type[i].compare(r2.r_schema.type[i]) != 0)
			{
			    cout<<"The Arity's do not match for the schemas"<<endl;
			    return false;
			}
        }
        return true;
    }
};

class project : public unaryoperators{  //Class for the projection operation
public:
	Relation r1;
	Relation func(vector<string> attr, Relation r)
    {
        r1.name = "Projection_of_";
        r1.name.append(r.name);
        for(unsigned int j = 0; j < attr.size();  j++)
        {
            if(!IsAnyArith(attr[j])){       //Is any arithmetic operation in the attribute
                int index;
               for(index = 0; index < r.r_schema.attr.size(); index++){
                    if(r.r_schema.attr[index] == attr[j]){
                            break;}}
               r1.r_schema.attr.push_back(attr[j]);
               r1.r_schema.type.push_back(r.r_schema.type[index]);
               if( j == 0){
                        for(unsigned int i=0; i < r.tupple.size();i++){
                            Record c;
                            c.element.push_back(r.tupple[i].element[index]);
                            r1.tupple.push_back(c);}}
               else{
                        for(unsigned int i=0; i < r1.tupple.size();i++)
                            r1.tupple[i].element.push_back(r.tupple[i].element[index]);}
            }
            else{
               r1.r_schema.attr.push_back(attr[j]);
               r1.r_schema.type.push_back("float");
               if( j == 0){
                        for(unsigned int i=0; i < r.tupple.size();i++){
                            Record c;
                            stringstream ss (stringstream::in | stringstream::out);
                            ss << compute(attr[j],r.r_schema,r.tupple[i]);
                            string test = ss.str();
                            c.element.push_back(test);
                            r1.tupple.push_back(c);}}
               else{
                        for(unsigned int i=0; i < r1.tupple.size();i++){
                            stringstream ss (stringstream::in | stringstream::out);
                            ss << compute(attr[j],r.r_schema,r.tupple[i]);
                            string test = ss.str();
                            r1.tupple[i].element.push_back(test);}}
            }
        }
        return r1;
    }
};

class rename_ : public unaryoperators{  //Class for the Rename operation
public:
        Relation r1;
	Relation func(Relation r, const char *x, vector <string> attr_names){
		r1 = r;
        	if(x != NULL && !attr_names.empty())    //Renaming attributes
        	{
            		if(attr_names.size() != r.r_schema.attr.size())
            		{
                		cout<<"Rename Error : Not all attributes are given"<<endl;
                		exit(0);
            		}
            		for(unsigned int i=0; i < r.r_schema.attr.size(); i++)
                		r1.r_schema.attr[i] = attr_names[i];
        	}
        	r1.name = x;
        	string fname = x;
        	fname.append(".dat");
        	writetofile(r1,fname.data());
        	return r1;
	}
};

class union_ : public binaryoperators{  //Class for the union operation
public:
	Relation r;
	Relation func(Relation r1, Relation r2)
	{
        unsigned int i;
        if(!checkarity(r1,r2)){ //Check for the arity of both the relations
            exit(0);
        }
        r.name = r1.name;
	    r.name.append("_union_");
	    r.name.append(r2.name);
        for(i=0; i < r1.r_schema.attr.size(); i++){
			r.r_schema.attr.push_back(r1.r_schema.attr[i]);
			r.r_schema.type.push_back(r1.r_schema.type[i]);
		}
		for(i = 0; i < r1.tupple.size(); i++)
			r.tupple.push_back(r1.tupple[i]);
		for(unsigned k = 0; k < r2.tupple.size(); k++)
			r.tupple.push_back(r2.tupple[k]);
	        r.remove_dup(); //remove duplicates in the final relation
		return r;
    }
};
class product : public binaryoperators{  //Class for the product operation
public:
    Relation r;
    Relation func(Relation r1, Relation r2)
    {
        r.name = r1.name;
	    r.name.append("_cross_");
	    r.name.append(r2.name);
        unsigned int i,l=0;
        for(i=0; i < r1.r_schema.attr.size(); i++){ //naming the attributes with the names of relations
            string temp = r1.name;
            temp.append(".");
            temp.append(r1.r_schema.attr[i]);
			r.r_schema.attr.push_back(temp);
			r.r_schema.type.push_back(r1.r_schema.type[i]);
		}
		for(i=0; i < r2.r_schema.attr.size(); i++){
		    string temp = r2.name;
            temp.append(".");
            temp.append(r2.r_schema.attr[i]);
			r.r_schema.attr.push_back(temp);
			r.r_schema.type.push_back(r2.r_schema.type[i]);
		}
		for(unsigned int j = 0; j < r2.tupple.size(); j++)  //concatinating each row of r1 with each row of r2
        {
            for(i=0; i < r1.tupple.size(); i++){
                l++;
                r.tupple.push_back(r1.tupple[i]);
                    for(int k =0; k < r2.tupple[j].element.size(); k++ )
                        r.tupple[l-1].element.push_back(r2.tupple[j].element[k]);
                }
        }
        r.remove_dup(); //remove duplicates in the final relation
        return r;
    }
};

class difference : public binaryoperators{  //Class for the Diffeerence operation
public:
	Relation r;
	Relation func(Relation r1, Relation r2){
	    if(!checkarity(r1,r2)){//naming the attributes with the names of relations
            exit(0);
        }
        r.name = r1.name;
	    r.name.append("_minus_");
	    r.name.append(r2.name);
		unsigned int i;
	    	for(i=0; i < r1.r_schema.attr.size(); i++){ //names of attributes same as r1
			r.r_schema.attr.push_back(r1.r_schema.attr[i]);
			r.r_schema.type.push_back(r1.r_schema.type[i]);
		}
		for(i = 0; i < r1.tupple.size(); i++){  //removing the tupples that are common
			int count = 0;
			for(unsigned k = 0; k < r2.tupple.size(); k++){
				int p = 0;
				for(int j=0; j<r2.tupple[k].element.size(); j++){
					if(r2.tupple[k].element[j].compare(r1.tupple[i].element[j]) != 0)
						p++;
				}
				if(p!=0)
					count++;
			}
			if(count == r2.tupple.size())
				r.tupple.push_back(r1.tupple[i]);
		}
		r.remove_dup();
		return r;
	}
};

typedef struct  //A structure for storing each term of a condition a sigma operation
{
    bool isTerm;
    char op;
    string lhs;
    string rhs;
    bool isRhsConst;
    string type;
}terms;

class select_ : public unaryoperators{  //Class for the Sigma operation
public:
    Relation r;

    bool parse(string s)        // A function which parses string in the form "T^(F|T)"  and returns the resulting truth value
    {
        bool ret = true;
        int j;
        bool wasThereNot = false;
        for(int i = 0 ; i < s.size(); i++)
        {
            if(s[i] == '^')
            {
                    i++;
                    if(s[i] == '!')
                    {
                        i++;
                         if(s[i] == 'T')
                        ret = ret && false;
                        else  if(s[i] == '(')
                        {
                            int k =0;
                            for(j = i+1 ; j<s.size();j++)
                            {
                                            if(s[j] == '(')
                                               k++;
                                            else    if(s[j] == ')' && k!=0)
                                               k--;
                                            else   if(s[j] == ')' && k==0)
                                                break;
                            }
                            ret = ret && !parse(s.substr(i+1,j-i-1));
                            i = j+1;
                        }
                        else    if(s[i] == 'F')
                            ret = ret && true;
                    }
                    else    if(s[i] == 'T')
                        ret = ret && true;
                    else  if(s[i] == '(')
                    {
                        int k =0;
                        for(j = i+1 ; j<s.size();j++)
                        {
                                        if(s[j] == '(')
                                           k++;
                                        else    if(s[j] == ')' && k!=0)
                                           k--;
                                        else   if(s[j] == ')' && k==0)
                                            break;
                        }
                        ret = ret && parse(s.substr(i+1,j-i-1));
                        i = j+1;
                    }
                    else    if(s[i] == 'F')
                        ret = ret && false;
            }
            else    if(s[i] == '|')
            {
                    i++;
                    if(s[i] == '!')
                    {
                        i++;
                         if(s[i] == 'T')
                        ret = ret || false;
                        else  if(s[i] == '(')
                        {
                            int k =0;
                            for(j = i+1 ; j<s.size();j++)
                            {
                                            if(s[j] == '(')
                                               k++;
                                            else    if(s[j] == ')' && k!=0)
                                               k--;
                                            else   if(s[j] == ')' && k==0)
                                                break;
                            }
                            ret = ret || !parse(s.substr(i+1,j-i-1));
                            i = j+1;
                        }
                        else    if(s[i] == 'F')
                            ret = ret || true;
                    }
                    else    if(s[i] == 'T')
                        ret = ret || true;
                    else  if(s[i] == '(')
                    {
                        int k = 0;
                        for(j = i+1 ; j<s.size();j++)
                        {
                                        if(s[j] == '(')
                                           k++;
                                        else    if(s[j] == ')' && k!=0)
                                           k--;
                                        else   if(s[j] == ')' && k==0)
                                            break;
                        }
                        ret = ret || parse(s.substr(i+1,j-i-1));
                        i = j+1;
                    }
                    else    if(s[i] == 'F')
                        ret = ret || false;
            }
            else    if(s[i] == '(')
            {
                int k =0;
                for(j = i+1 ; j<s.size();j++)
                {
                                if(s[j] == '(')
                                   k++;
                                else    if(s[j] == ')' && k!=0)
                                   k--;
                                else   if(s[j] == ')' && k==0)
                                    break;
                }
                if(wasThereNot){
                    ret = !parse(s.substr(i+1,j-i-1));}
                else
                    ret = parse(s.substr(i+1,j-i-1));
                i = j+1;
            }
            else    if(s[i] == 'T')
            {
                if(wasThereNot)
                    ret = false;
                else
                    ret = true;
                 wasThereNot = false;
            }
            else    if(s[i] == 'F')
            {
                if(wasThereNot)
                    ret = true;
                else
                    ret = false;
                wasThereNot = false;
            }
            else    if(s[i] == '!')
            {
                if(wasThereNot)
                    wasThereNot = false;
                else
                    wasThereNot = true;
            }
        }
        return ret;
    }

    char retval(float value, float compval, char op)
    {
        switch(op)
        {
            case('=') :     if(value == compval)
                                    return 'T';
                            else
                                    return 'F';
            case('N') :    if(value != compval)
                                    return 'T';
                            else
                                    return 'F';
            case('>') :     if(value > compval)
                                    return 'T';
                            else
                                    return 'F';
            case('<') :     if(value < compval)
                                    return 'T';
                            else
                                    return 'F';
            case('G') :     if(value >= compval)
                                    return 'T';
                            else
                                    return 'F';
            case('L') :     if(value <= compval)
                                    return 'T';
                            else
                                    return 'F';
        }
    }

    char evalaute(Record t1,Schema s1,terms TRM)    //Evaluating the arithmetic operation for each row
    {
        bool arith = IsAnyArith(TRM.lhs);
        if(TRM.isRhsConst){ //Rhs is a constant not attribute
            if(TRM.type.compare("string") == 0){
                for(int i = 0; i < t1.element.size(); i++ ){
                        if(s1.attr[i].compare(TRM.lhs) == 0){
                                if( s1.type[i].compare("string") == 0){//Compare strings
                                if((t1.element[i].compare(TRM.rhs) == 0 && TRM.op == '=') || (t1.element[i].compare(TRM.rhs) != 0 && TRM.op == 'N'))
                                    return 'T';
                                else
                                    return 'F';
                                }
                                else{//The types does not match in sigma condition
                                    cout<<"Error: The types does not match in sigma condition"<<endl;
                                    cout<<s1.type[i];
                                    exit(0);
                                }
                        }
                }
            }
            else    if(TRM.type.compare("int") == 0){   //Comparing ints
                int compval,value;
                value = (int)compute(TRM.lhs,s1,t1);    //compute LHS and RHS terms
                compval = atoi(TRM.rhs.data());
                return (retval(value,compval,TRM.op));
            }
            else    if(TRM.type.compare("float") == 0)  //Comparing floats
            {
                float compval,value;
                value = compute(TRM.lhs,s1,t1);
                compval = atoi(TRM.rhs.data());
                return (retval(value,compval,TRM.op));
            }
        }
        else
        {
            int temp,temp1;
            float compval,value;
            if(!arith && !IsAnyArith(TRM.rhs))
            {
                for(int i = 0; i < t1.element.size(); i++)
                {
                    if(s1.attr[i] == TRM.lhs)
                        temp = i;
                     if(s1.attr[i] == TRM.rhs)
                        temp1 = i;
                }
                if(s1.type[temp] != s1.type[temp1]){
                cout<<"Error:Types do not match in sigma"<<endl;
                exit(0);
                }
                else    if(s1.type[temp].compare("string")){
                                if((t1.element[temp].compare(t1.element[temp1]) == 0 && TRM.op == '=') || (t1.element[temp].compare(t1.element[temp1]) != 0 && TRM.op == 'N'))
                                    return 'T';
                                else
                                    return 'F';
                }
                else{
                    value = compute(t1.element[temp],s1,t1);
                    compval = compute(t1.element[temp1],s1,t1);
                    return (retval(value,compval,TRM.op));
                }
            }
            else{
                value = (int)compute(TRM.lhs,s1,t1);
                compval = (int) compute(TRM.rhs,s1,t1);
                return (retval(value,compval,TRM.op));
            }
        }
    }
    string changeexp(Record t1, Schema s1,vector<terms> expression){    //Change the given expression in the form "T^(T|F)"
        char s[expression.size() + 1];
        int j = 0;
        for(unsigned int i = 0; i < expression.size(); i++){
                if(expression[i].isTerm == false)
                    s[j] = expression[i].op;
                else
                    s[j] = evalaute(t1,s1,expression[i]);
                j++;
        }
        s[j] = '\0';
        return s;
    }

    Relation func(Relation r1,vector<terms> expression) //The function for Sigma operation
    {
        string exp;
        r.name = "Selection_of_";
        r.name.append(r1.name);
        for(int i=0; i < r1.r_schema.attr.size(); i++){
			r.r_schema.attr.push_back(r1.r_schema.attr[i]);
			r.r_schema.type.push_back(r1.r_schema.type[i]);
		}
        for(int i = 0; i < r1.tupple.size(); i++){
            exp = changeexp(r1.tupple[i],r1.r_schema,expression);
            if(parse(exp))
                r.tupple.push_back(r1.tupple[i]);
        }
        r.remove_dup();
        return r;
    }
};
