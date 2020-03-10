#ifndef SET_H
#define SET_H

#include <ostream>	//operator <<
#include <cstddef>	//ptrdiff_t
#include <algorithm> //swap
#include <iterator>	//const_iterator
#include <cassert>	//assert

/**
	@file set.h 
	@brief Dichiarazione della classe set
**/

/**
	@brief Dichiarazione dell'eccezione already_existing_exception

	L'eccezione viene lanciata quando si tenta di aggiungere in un set
	un elemento già esistente (nel set stesso).
**/
class already_existing_exception{};

/**
	@brief Dichiarazione dell'eccezione not_existing_exception

	L'eccezione viene lanciata quando si tenta di rimuovere un elemento
	non esistente in un set.
**/
class not_existing_exception{};

/**
	@brief Set di elementi generici

	Classe che rappresenta un set di elementi generici.
	Il set è stato implementato con una lista doppiamente linkata.
**/
template <typename T, typename Eql>
class set{

private:
	typedef unsigned int size_type;	///< Definzione del tipo corrispondente a size
	
	/**
		@brief Singolo nodo della lista

		Struttura che definisce il singolo elemento della lista doppiamente linkata.
	**/
	struct node{
		T value;	///< Dato di tipo generico T
		node *previous;	///< Puntatore all'elemento precedente della lista
		node *next;	///< Puntatore all'elemento successivo della lista	

		/**
		@brief Costruttore secondario

			Costruttore secondario. Permette di istanziare un nodo con un dato valore.
			@param v Valore con cui inizializzare il nodo.
			@param prev Puntatore al nodo precedente, di default è 0.
			@param succ Puntatore al nodo successivo, di default è 0.
		**/
		node(const T &v, node *prev=0, node *succ=0) : 
			value(v), previous(prev), next(succ) {}
	};

	node *_head;	///< Puntatore alla testa della lista di dati di tipo generico T
	node *_tail;	///< Puntatore alla coda della lista di dati di tipo generico T
	size_type _size;	///< Dimensione della lista
	Eql _equal;		///< Definizione del tipo di comparazione uguaglianza

	/**
		@brief Ricerca di un elemento nel set

		Ricerca per valore un elemento nel set e ne restituisce un puntatore.
		Se l'elemento non è presente nel set, ritorna 0.
		@param v Valore da ricercare nel set.
		@return Puntatore all'elemento ricercato.
	**/
	node *search(const T &v) const{

		node *tmp = _head;

		while(tmp != 0 && !_equal(tmp->value, v))
			tmp = tmp->next;
		
		return tmp;
	}

public:
	/**
		@brief Costruttore di default

		Costruttore di default per istanziare un set vuoto.
	**/
	set() : _head(0), _tail(0), _size(0) {}

	/**
		@brief Costruttore secondario (COSTRUTTORE DI COPIA)

		Costruttore secondario. Permette di creare un set come copia
		di un altro set.
		@param other Set sorgente.
	**/
	set(const set &other) : _head(0), _tail(0), _size(0) {

		node *tmp = other._head;
		try{
			while(tmp != 0){
				add(tmp->value);
				tmp = tmp->next;
			}
		} catch(...){
			clear_set();
			throw;
		}
	}

	/**
		@brief Costruttore secondario (COSTRUTTORE GENERICO)

		Costruttore secondario. Permette di creare un set a partire da una sequenza
		di dati definita da una coppia generica di iteratori.
		@param b Iteratore all'inizio della sequenza di dati.
		@param e Iteratore alla fine della sequenza di dati.
	**/
	template <typename Q>
	set(Q b, Q e) : _head(0), _tail(0), _size(0) {
		try{
			for(; b!=e; ++b)
				add(static_cast<T>(*b));
		} catch (...){
			clear_set();
			throw;
		}
	}

	/**
		@brief Distruttore

		Distruttore. Rimuove la memoria allocata da set.
	**/
	~set(){
		clear_set();
	}

	/**
		@brief Operatore di assegnamento

		Operatore di assegnamento. Permette la copia tra set.
		@param other Set sorgente.
		@return Riferimento a this.
	**/
	set &operator=(const set &other){

		if(this != &other){
			set tmp(other);
			std::swap(tmp._head, _head);
			std::swap(tmp._tail, _tail);
			std::swap(tmp._size, _size);
		}
		return *this;
	}

	/**
		@brief Accesso ai dati in sola lettura

		Metodo per leggere l'index-esimo elemento del set.

		@pre E' necessario che index < size.
		@param index Indice dell'elemento nel set da leggere.
		@return Il valore dell'elemento in posizione index-esima.
	**/
	const T &operator[](const size_type index) const {

		assert(index < _size);

		size_type count = 0;
		node *tmp = _head;

		while(tmp != 0 && count < index){
			tmp = tmp->next;
			count++;
		}
		return tmp->value;
	}

	/**
		@brief Svuota il set

		Metodo che itera sul set ed elimina tutti gli elementi liberando la memoria. 
	**/
	void clear_set(void){
		node *tmp = _head;

		while(tmp != 0){
			node *next = tmp->next;
			_head = next;
			delete tmp;
			if(_head == 0)
				_tail = 0;
			_size--;
			tmp = next;
		}
	}
	
	/**
		@brief Aggiunge un elemento al set

		Metodo che aggiunge un elemento, se non è già esistente, al set.
		@param value Il valore da aggiungere al set.
		@throw already_existing_exception Eccezione che viene lanciata
		in caso di elemento già esistente nel set.
	**/
	void add(const T &value){

		node *new_node = new node(value);

		//caso in cui il set è vuoto
		if(_head == 0){
			_head = new_node;
			_tail = new_node;
			_size++;
			return;
		}
		
		//caso in cui ho almeno un elemento nel set
		if(search(value) == 0){
			//aggiungo l'elemento in coda
			new_node->previous = _tail;	
			_tail->next = new_node;
			_tail = new_node;
        	_size++;
		}
		else{
			//l'elemento è già esistente
			delete new_node;
			new_node = 0;
			throw already_existing_exception();
		}
	}

	/**
		@brief Rimuove un elemento dal set

		Metodo che rimuove un elemento, se esistente, dal set.
		@param value Il valore da rimuovere dal set.
		@throw not_existing_exception Eccezione che viene lanciata
		in caso di elemento non esistente nel set.
	**/
	void remove(const T &value){
		
		node *del_node = search(value);

		if(del_node != 0){	
			//l'elemento da cancellare esiste nel set
			if(del_node == _head){
				//l'elemento da cancellare è in testa
				_head = del_node->next;
				if(_head != 0)
					//il set è composto da almeno due elementi
					_head->previous = 0;
				else
					//il set è composto da un solo elemento
					_tail = 0;

				delete del_node;
				del_node = 0;
    			_size--;
    			return;
			}
			//l'elemento da cancellare non è in testa
   			del_node->previous->next = del_node->next;
    			if (del_node->next != 0)
        			del_node->next->previous = del_node->previous;
        		else
        			_tail = del_node->previous;
        	delete del_node;
        	del_node = 0;
    		_size--;
		}
		else
			//l'elemento da cancellare non esiste nel set
			throw not_existing_exception();
	}

	/**
		@brief Dimensione del set 

		Ritorna il numero di elementi contenuti nel set.
		@return La dimensione del set.
	**/
	size_type size(void) const{
		return _size;
	}

	/**
		@brief Definizione della classe const_iterator

		Classe che implementa iteratori di tipo costante
		utilizzabili nella classe set.
	**/
	class const_iterator {
		const node *n;
	public:
		typedef std::forward_iterator_tag iterator_category;
		typedef T                         value_type;
		typedef ptrdiff_t                 difference_type;
		typedef const T*                  pointer;
		typedef const T&                  reference;

		/**
			@brief Costruttore di default

			Costruttore di default per istanziare un const_iterator.
		**/
		const_iterator() : n(0){}
		
		/**
			@brief Costruttore secondario (COSTRUTTORE DI COPIA)
	
			Costruttore secondario. Permette di creare un const_iterator
			come copia di un altro const_iterator.
			@param other Const_iterator sorgente.
		**/
		const_iterator(const const_iterator &other) : n(other.n){}

		/**
			@brief Operatore di assegnamento

			Operatore di assegnamento. Permette la copia tra const_iterator.
			@param other Const_iterator sorgente.
			@return Riferimento a this.
		**/
		const_iterator& operator=(const const_iterator &other) {
			n = other.n;
			return *this;
		}

		/**
			@brief Distruttore

			Distrugge il const_iterator al termine dell'utilizzo.
		**/
		~const_iterator() {}

		/**
			@brief Operatore di dereferenziamento

			Operatore di dereferenziamento. Ritorna il dato riferito dall'iteratore.
			@return Il dato riferito dall'iteratore.
		**/
		reference operator*() const {
			return n->value;
		}

		/**
			@brief Operatore freccia

			Operatore freccia. Ritorna il puntatore al dato riferito dall'iteratore
			@return Il puntatore al dato riferito dall'iteratore
		**/
		pointer operator->() const {
			return &(n->value);
		}
		
		/**
			@brief Operatore di iterazione post-incremento 

			Operatore di iterazione post-incremento. Incrementa il const_iteratore
			ma ne ritorna la copia non iterata.
			@return La copia del const_iterator.
		**/
		const_iterator operator++(int) {

			const_iterator tmp(*this);
			n = n->next;
			return tmp;
		}

		/**
			@brief Operatore di iterazione pre-incremento 

			Operatore di iterazione pre-incremento. Incrementa il const_iteratore
			ma ne ritorna il riferimento
			@return Il riferimento al const_iterator.
		**/
		const_iterator& operator++() {
			n = n->next;
			return *this;
		}

		/**
			@brief Operatore di uguaglianza

			Operatore di uguaglianza. Esegue una comparazione tra due const_iterator,
			se sono uguali restituisce true, altrimenti restituisce false.
			@param other Il const_iterator con cui effettuare la comparazione.
			@return Il risultato della comparazione di due const_iterator.
		**/
		bool operator==(const const_iterator &other) const {
			return (n==other.n);
		}
		
		/**
			@brief Operatore di diversità

			Operatore di diversità. Esegue una comparazione tra due const_iterator,
			se sono diversi restituisce true, altrimenti restituisce false.
			@param other Il const_iterator con cui effettuare la comparazione.
			@return Il risultato della comparazione di due const_iterator.
		**/
		bool operator!=(const const_iterator &other) const {
			return (n!=other.n);
		}

	private:

		friend class set;
		const_iterator(const node *nn) : n(nn){}		
	}; // classe const_iterator
	
	/**
		@brief Iteratore all'inizio della sequenza di dati

		@return L'iteratore all'inizio della sequenza di dati.
	**/
	const_iterator begin() const {
		return const_iterator(_head);
	}
	
	/**
		@brief Iteratore alla fine della sequenza di dati
	
		@return L'iteratore alla fine della sequenza di dati.
	**/
	const_iterator end() const {
		return const_iterator(0);
	}
};

/**
	@brief Operatore di stream

	Permette di spedire su uno stream di output il contenuto del set generico.
	Questo metodo è templato in quanto il set in questione è di tipo templato.
	@param os stream di output
	@param data_set Set sorgente i cui elementi verranno spediti sullo stream.
	@return Il riferimento allo stream di output.
**/
template <typename T, typename E>
std::ostream &operator<<(std::ostream &os, const set<T, E> &data_set){

	typename set<T, E>::const_iterator i, ie;

	for(i=data_set.begin(), ie=data_set.end(); i!=ie; ++i)
		os << *i << " ";
	return os;
}

/**
	@brief Filtra un set attraverso l'uso di un predicato

	Funzione globale che dato un set generico e un predicato booleano generico, ritorna un
	nuovo set ottenuto prendendo dal set sorgente tutti gli elementi che non
	soddisfano il predicato.
	Questo metodo è templato in quanto il set in questione è di tipo templato.
	Il tipo templato Pred definisce il tipo del predicato.
	@param S Set sorgente.
	@param P Predicato applicato al set sorgente.
	@return Il set risultato dell'applicazione del predicato al set sorgente.
**/
template <typename T, typename Eql, typename Pred>
set<T, Eql> filter_out(const set<T, Eql> &S, const Pred P){
	
	typename set<T, Eql>::const_iterator i, ie;
	set<T, Eql> new_set;

	for(i=S.begin(), ie=S.end(); i!=ie; ++i)

		if(!P(*i))
			new_set.add(*i);

	return new_set;
}

/**
	@brief Concatenzione di due set

	Funzione globale che dati in input due set generici, ritorna un set che contiene 
	gli elementi di entrambi i set.
	Viene mantenuta l'unicità degli elementi quindi se l'intersezione dei due set
	non è vuota, viene lanciata un'eccezione.
	@param first Primo set sorgente.
	@param second Secondo set sorgente.
	@throw already_existing_exception Eccezione che viene lanciata
	in caso di elemento già esistente nel set.
	@return Il set risultato della concatenzione dei due set sorgenti.
**/
template <typename T, typename Eql>
set<T, Eql> operator+(const set<T, Eql> &first, const set<T, Eql> &second){

	typename set<T, Eql>::const_iterator i, ie;
	set<T, Eql> new_set(first);

	for(i=second.begin(), ie=second.end(); i!=ie; ++i){

		try{
			new_set.add(*i);
		}catch(already_existing_exception e){		
			throw e;			
		}
	}
	return new_set;
}
#endif
