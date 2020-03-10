#include "set.h"
#include <iostream>
#include <cassert>
#include <string>

/**
	@brief Definizione del funtore per l'uguaglianza tra interi

	Funtore che definisce il criterio con cui vengono comparate
	due variabili di tipo intero per valutarne l'uguaglianza.
**/
struct equal_int{

	bool operator()(const int a, const int b) const {
		return a == b;
	}		
};

/**
	@brief Definizione del funtore per il controllo della disparità

	Funtore che definisce il criterio con cui vengono comparate
	due variabili di tipo intero per valutarne la disparità.
**/
struct is_odd{
	
	bool operator()(const int a) const {
		return (a%2) != 0;
	}
};

/**
	@brief Definizione del funtore per l'uguaglianza tra stringhe

	Funtore che definisce il criterio con cui vengono comparate
	due variabili di tipo stringa per valutarne l'uguaglianza.
**/
struct equal_string{

	bool operator()(const std::string &str1, 
					const std::string &str2) const {
		return str1 == str2;
	}		
};

/**
	@brief Definizione del funtore per il controllo della lunghezza della stringa

	Funtore che controlla se la lunghezza della stringa 
	presa come parametro d'ingresso sia uguale a 5.
**/
struct string_length_5{
	
	bool operator()(const std::string &str) const {
		return str.length() == 5;
	}
};

/**
	@brief Definizione della struttura che rappresenta una voce

	Definizione di una voce, da considerarsi come una voce di una rubrica telefonica,
	contenente tre stringhe come attributi membro.
**/
struct voce{
	std::string nome;
	std::string cognome;
	std::string ntel;

	/**
		@brief Costruttore secondario

		Costruttore secondario. Permette di istanziare una voce con un nome,
		un cognome e un numero di telefono.
		@param n Il nome da assegnare alla voce.
		@param cg Il cognome da assegnare alla voce.
		@param nt Il numero di telefono da assegnare alla voce.
	**/
	voce(const std::string &n, const std::string &cg, const std::string &nt)
	: nome(n), cognome(cg), ntel(nt) {}

	/**
		@brief Definizione dell'operatore di uguaglianza

		Operatore di uguaglianza. Definisce il criterio di comparazione
		tra due voci.
		@param other La voce con cui effettuare la comparazione.
		@return Il risultato (booleano) della comparazione.
	**/
	bool operator==(const voce &other) const{
	return (this->ntel == other.ntel && 
			this->nome == other.nome &&
			this->cognome == other.cognome);
	}
};

/**
	@brief Operatore di stream

	Permette di spedire su uno stream di output il contenuto di una voce.
	@param os stream di output
	@param v Voce sorgente i cui elementi verranno spediti sullo stream.
	@return Il riferimento allo stream di output.
**/
std::ostream &operator<<(std::ostream &os, const voce &v){
	os << "[Nome: " << v.nome << ", Cognome: " << v.cognome << ", Num: " << v.ntel << "]";
	os << std::endl;
	return os;
}

/**
	@brief Definizione del funtore per l'uguaglianza tra voci

	Funtore che definisce il criterio con cui vengono comparate
	due variabili di tipo "voce" per valutarne l'uguaglianza.
**/
struct equal_voce{

	bool operator()(const voce &v1, const voce &v2) const {
		return v1 == v2;		
	}		
};

/**
	@brief Definizione del funtore per il controllo del cognome di una voce

	Funtore che controlla se il cognome di una voce sia uguale a "Rossi".
	@param v Voce da controllare.
	@return Il risultato (booleano) della valutazione del cognome.
**/
struct voce_surname_Rossi{
	
	bool operator()(const voce &v) const {
		return (v.cognome == "Rossi");
	}
};

void test_int(){
	typedef set<int, equal_int> set_int_type;
	set_int_type set1;			//default constructor
	set_int_type set2(set1);	//copy-constructor

	try{
		set1.add(5);		//add(), search()
		set1.add(4);		
		set1.add(23);
		set1.add(-56);
		set1.add(1);
		set1.add(-9);
					
		set1.add(5);		//already_existing_exception

	}catch(already_existing_exception){
		std::cout << "already_existing_exception CATCHED" << std::endl;
	};

	assert(set1[0] == 5);	//operator[]
	assert(set1[1] == 4);
	assert(set1[2] == 23);
	assert(set1[3] == -56);
	std::cout << set1 << std::endl;		//operator <<, const_iterator

	set_int_type::const_iterator i, ie;		//const_iterator
	i = set1.begin();
	ie = set1.end();
	set_int_type set_iterators(i, ie);		//set(Q b, Q e)

	assert(set_iterators[0] == 5);	//operator[]
	assert(set_iterators[1] == 4);
	assert(set_iterators[2] == 23);
	assert(set_iterators[3] == -56);

	set2 = set1;					//operator=
	std::cout << set2 << std::endl;

	try{	
		set1.remove(5);		//head deletion
		set1.remove(-56);	//tail deletion
		set1.add(56);
		set1.add(57);
		set1.remove(56);	//middle deletion
				
		set1.remove(565656);	//not_existing_exception

	}catch(not_existing_exception){
		std::cout << "not_existing_exception CATCHED" << std::endl;
	};

	set1.clear_set();	//clear_set()
	assert(set1.size() == 0);	//size()

	set_int_type filtered;
	filtered = filter_out(set2, is_odd());		//filter_out
	assert(filtered[0] == 4);
	assert(filtered[1] == -56);

	set1.add(1010);
	set1.add(-999);
	set1.add(0);

	set_int_type concat;
	concat = set1 + set2;				//operator+
	assert(concat[0] == 1010);
	assert(concat[8] == -9);

	std::cout << "test_int() OK" << std::endl;
	std::cout << "---------------------" << std::endl;
}
void test_string(){
	typedef set<std::string, equal_string> set_string_type;
	set_string_type set1;			//default constructor
	set_string_type set2(set1);		//copy-constructor

	try{
		set1.add("Mario");		//add(), search()
		set1.add("Giovanni");		
		set1.add("Luca");
		set1.add("Lucia");
		set1.add("Sara");
		set1.add("Deborah");

		set1.add("Mario");		//already_existing_exception

	}catch(already_existing_exception){
		std::cout << "already_existing_exception CATCHED" << std::endl;
	};
	
	assert(set1[0] == "Mario");	//operator[]
	assert(set1[1] == "Giovanni");
	assert(set1[2] == "Luca");
	assert(set1[3] == "Lucia");
	std::cout << set1 << std::endl;		//operator <<, const_iterator

	set_string_type::const_iterator i, ie;		//const_iterator
	i = set1.begin();
	ie = set1.end();
	set_string_type set_iterators(i, ie);		//set(Q b, Q e)

	assert(set_iterators[0] == "Mario");	//operator[]
	assert(set_iterators[1] == "Giovanni");
	assert(set_iterators[2] == "Luca");
	assert(set_iterators[3] == "Lucia");

	set2 = set1;					//operator=
	std::cout << set2 << std::endl;

	try{
		set1.remove("Mario");		//head deletion
		set1.remove("Lucia");		//tail deletion
		set1.add("Matteo");
		set1.add("Gianni");
		set1.remove("Matteo");		//middle deletion

		set1.remove("Piero");		//not_existing_exception
		
	}catch(not_existing_exception){
		std::cout << "not_existing_exception CATCHED" << std::endl;
	};

	set1.clear_set();	//clear_set()
	assert(set1.size() == 0);	//size()

	set_string_type filtered;
	filtered = filter_out(set2, string_length_5());		//filter_out
	assert(filtered[0] == "Giovanni");
	assert(filtered[1] == "Luca");
	assert(filtered[2] == "Sara");
	assert(filtered[3] == "Deborah");

	set1.add("Mattia");
	set1.add("Leonardo");
	set1.add("Fabio");

	set_string_type concat;
	concat = set1 + set2;				//operator+
	assert(concat[4] == "Giovanni");
	assert(concat[6] == "Lucia");

	std::cout << "test_string() OK" << std::endl;
	std::cout << "---------------------" << std::endl;
}

void test_voce(){
	typedef set<voce, equal_voce> set_voce_type;
	set_voce_type set1;			//default constructor
	set_voce_type set2(set1);		//copy-constructor
	
	voce v1("Mario", "Rossi", "6959595");
	voce v2("Luca", "Rossi", "8855855");
	voce v3("Lucia", "Rossi", "123455");
	voce v4("Sara", "Verdi", "987654");
	voce v5("Deborah", "Verdi", "2548963");

	try{
		set1.add(v1);		//add(), search()
		set1.add(v2);		
		set1.add(v3);
		set1.add(v4);
		set1.add(v5);

		set1.add(v1);	//already_existing_exception

	}catch(already_existing_exception){
		std::cout << "already_existing_exception CATCHED" << std::endl;
	};
	
	assert(set1[0] == v1);	//operator[]
	assert(set1[1] == v2);
	assert(set1[4] == v5);
	std::cout << set1 << std::endl;		//operator <<, const_iterator

	set_voce_type::const_iterator i, ie;		//const_iterator
	i = set1.begin();
	ie = set1.end();
	set_voce_type set_iterators(i, ie);		//set(Q b, Q e)

	assert(set_iterators[0] == v1);	//operator[]
	assert(set_iterators[1] == v2);
	assert(set_iterators[2] == v3);
	assert(set_iterators[3] == v4);

	set2 = set1;					//operator=
	std::cout << set2 << std::endl;

	try{
		set1.remove(v1);		//head deletion
		set1.remove(v5);		//tail deletion
		set1.remove(v3);		//middle deletion

		set1.remove(voce("Piero", "Neri", "98966333"));	//not_existing_exception
		
	}catch(not_existing_exception){
		std::cout << "not_existing_exception CATCHED" << std::endl;
	};

	set1.clear_set();	//clear_set()
	assert(set1.size() == 0);	//size()

	set_voce_type filtered;
	filtered = filter_out(set2, voce_surname_Rossi());		//filter_out
	assert(filtered[0] == v4);
	assert(filtered[1] == v5);
	assert(filtered.size() == 2);

	set1.add(voce("Mattia", "Bianchi", "1111111"));
	set1.add(voce("Leonardo", "Bianchi", "2222222"));
	set1.add(voce("Fabio", "Bianchi", "3333333"));

	set_voce_type concat;
	concat = set1 + set2;				//operator+
	assert(concat[1] == voce("Leonardo", "Bianchi", "2222222"));
	assert(concat[4] == v2);
	assert(concat[7] == v5);
	assert(concat.size() == 8);

	std::cout << "test_voce() OK" << std::endl;
	std::cout << "---------------------" << std::endl;
}

int main(void){

	test_int();
	test_string();
	test_voce();
	return 0;
}