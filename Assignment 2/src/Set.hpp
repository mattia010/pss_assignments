/**
 * @file set.hpp
 * 
 * @brief File header della classe template Set
 * 
 * File di dichiarazione e definizione della classe template Set
*/

#ifndef SRC_SET_HPP_
#define SRC_SET_HPP_

#define DEFAULT_SET_DIM 5    // dimensione predefinita di un set

#include <iterator>
#include <fstream>
#include <string>

/**
 * @brief Classe template Set
 * 
 * La classe template Set implementa un set generico su elementi di tipo T.
 * Un set non può contenere elementi duplicati.
*/
template<typename T>
class Set {
    T *ptr;         ///< puntatore alla prima cella del set
    int capacity;   ///< numero di celle del set in memoria (celle occupate + celle libere)

    // raddoppia la dimensione in memoria del set
    void expand() {
      capacity *= 2;
      T *ptr_tmp = new T[capacity];
      for (int i = 0; i < size; ++i) {
        ptr_tmp[i] = ptr[i];
      }
      delete[] ptr;
      ptr = ptr_tmp;
    }

    // dimezza la dimensione in memoria del set
    void compact() {
      if (size < (capacity / 2)) {
        capacity /= 2;
        T *ptr_tmp = new T[capacity];
        for (int i = 0; i < size; ++i) {
          ptr_tmp[i] = ptr[i];
        }
        delete[] ptr;
        ptr = ptr_tmp;
      }
    }

 public:
    int size;       ///< numero di elementi contenuti, quindi di celle del set effettivamente occupate

    /**
     * Costruttore di default
     * 
     * @post il set viene inizializzato a una dimensione di default
     * 
     * @throw std::bad_alloc
    */
    Set() : ptr(new T[DEFAULT_SET_DIM]), capacity(DEFAULT_SET_DIM), size(0) {}

    /**
     * Costruisce un set di dimensione pari al parametro.
     * 
     * @param capacityParam dimensione del set
    */
    explicit Set(const int capacityParam) : ptr(new T[capacityParam]), capacity(capacityParam), size(0) {}

    /**
     * Costruisce un set utilizzando due iteratori (begin e end) su un'altra struttura 
     * container.
     * Gli iteratori possono essere di qualsiasi tipo.
     * 
     * @param begin_itr iteratore di inizio sequenza
     * @param end_itr iteratore di fine sequenza
     * 
     * @throw std::bad_alloc
    */
    template<typename Q>
    Set(Q begin_itr, Q end_itr) : ptr(new T[DEFAULT_SET_DIM]), capacity(DEFAULT_SET_DIM), size(0) {
      try {
        for (Q curr_itr = begin_itr; curr_itr != end_itr; ++curr_itr) {
          add(static_cast<T>(*curr_itr));
        }
      } catch (...) {
        delete[] ptr;
        ptr = nullptr;
        capacity = 0;
        size = 0;
        throw;
      }
    }

    /**
     * Costruttore di copia
     * 
     * @param other altro set che si vuole copiare
     * 
     * @throw std::bad_alloc
    */
    Set(const Set &other) : ptr(new T[other.capacity]), capacity(other.capacity), size(other.size) {
      for (int i = 0; i < other.size; ++i) {
        ptr[i] = other[i];
      }
    }

    /**
     * Assegnamento per copia
     * 
     * @param other altro set che si vuole copiare
     * 
     * @return reference al set puntato da this
     * 
     * @throw std::bad_alloc
    */
    Set& operator=(const Set &other) {
      capacity = other.capacity;
      size = other.size;
      delete[] ptr;
      ptr = new T[capacity];
      for (int i = 0; i < other.size; ++i) {
        ptr[i] = other[i];
      }
      return *this;
    }

    /**
     * Distruttore di default
     * 
     * @post memoria completamente deallocata
    */
    ~Set() {
      delete[] ptr;
    }

    /**
     * Aggiunge un elemento al set solo se non è già contenuto.
     * Se il set è pieno la sua dimensione viene automaticamente aumentata.
     * 
     * @param element elemento che si vuole inserire
     * 
     * @throw std::bad_alloc
    */
    void add(const T &element) {
      if (!contains(element)) {
        if (size == capacity) {
          expand();
        }
        ptr[size] = element;
        ++size;
      }
    }


    /**
     * Rimuove un elemento dal set, se presente.
     * La dimensione in memoria viene poi automaticamente diminuita se necessario.
     * 
     * @param element elemento da rimuovere
     * 
     * @throw std::bad_alloc
    */
    void remove(const T &element) {
      for (int i = 0; i < size; ++i) {
        if (ptr[i] == element) {
          for (int j = i; j < size - 1; ++j) {
            ptr[j] = ptr[j + 1];
          }
          --size;
          if (size < (capacity / 4)) {
            compact();
          }
          // una volta trovato un elemento si è sicuri che non ce ne sono
          // altri uguali perchè è un set. Si può quindi ritornare.
          return;
        }
      }
    }

    /**
     * Verifica se un elemento è presente nel set.
     * 
     * @param element elemento di cui si vuole verificare la presenza
     * 
     * @return true se elemento è presente nel set, false altrimenti.
    */
    bool contains(const T &element) const {
      for (int i = 0; i < size; ++i) {
        if (ptr[i] == element) {
          return true;
        }
      }
      return false;
    }

    /**
     * Ritorna l'elemento contenuto alla cella di indice specificato.
     * 
     * @param index indice a cui si vuole accedere
     * 
     * @return reference costante all'elemento
     * 
     * @throw std::out_of_range se viene specificato come parametro un indice non accettabile
    */
    const T& operator[](const int index) const {
      if (index >= 0 && index < size) {
        return ptr[index];
      } else {
        throw std::out_of_range("You tried to access memory outside the set!");
      }
    }

    /**
     * Verifica se due set sono uguali. 
     * Due set sono uguali se contengono gli stessi elementi, 
     * anche se in un ordine differente.
     * 
     * @param other set da confrontare con this
     * 
     * @return true se i due set sono uguali, false altrimenti
    */
    bool operator==(const Set &other) const {
      if (size != other.size) {
        return false;
      }
      for (int i = 0; i < size; ++i) {
        if (!other.contains(ptr[i])) {
          return false;
        }
      }
      for (int i = 0; i < other.size; ++i) {
        if (!contains(other[i])) {
          return false;
        }
      }
      return true;
    }


    /**
     * Stampa un set su uno stream output.
     * 
     * @param os stream output
     * @param set set da stampare
     * 
     * @return reference allo stream output 
    */
    // è friend in modo da poter fare riferimento alla classe set senza specificare il tipo
    // generico T (che invece andrebbe fatto se fosse una funzione globale non friend esterna
    // alla classe) e anche perchè in questo modo è possibile accedere ai membri privati
    // della classe Set.
    friend std::ostream& operator<<(std::ostream &os, const Set &set) {
      os << set.size;
      for (const_iterator itr = set.begin(); itr != set.end(); ++itr) {
        os << " (" << *itr << ")";
      }
      os << std::endl;
      return os;
    }

    /**
     * Iteratore costante (sola lettura) sugli elementi di un set
    */
    class const_iterator {
     private:
        const T *it_ptr;  ///< puntatore all'elemento riferito dall'iteratore

        friend class Set;  // serve per poter richamare il costruttore privato dalla classe set
        explicit const_iterator(const T *elem) : it_ptr(elem) {}

     public:
        typedef std::forward_iterator_tag iterator_category;
        typedef T                         value_type;
        typedef ptrdiff_t                 difference_type;
        typedef const T*                  pointer;
        typedef const T&                  reference;

        // Costruttore di default
        const_iterator() : it_ptr(nullptr) {}

        const_iterator(const const_iterator &other) : it_ptr(other.it_ptr) {}

        // copy constructor
        const_iterator& operator=(const const_iterator &other) {
          it_ptr = other.it_ptr;
          return *this;
        }

        // Distruttore di default
        ~const_iterator() {}

        // ritorna l'oggetto a cui l'iteratore punta
        reference operator*() const {
          return *it_ptr;
        }

        // ritorna un puntatore all'oggetto a cui l'iteratore punta
        pointer operator->() const {
          return &(*it_ptr);
        }

        // post-incremento
        const_iterator operator++(int) {
          const_iterator tmp(*this);
          ++it_ptr;
          return tmp;
        }

        // pre-incremento
        const_iterator& operator++() {
          ++it_ptr;
          return *this;
        }

        // verifica se due iteratori puntano allo stesso oggetto
        bool operator==(const const_iterator &other) const {
          return it_ptr == other.it_ptr;
        }

        // verifica se due iteratori puntano a oggett diversi
        bool operator!=(const const_iterator &other) const {
          return it_ptr != other.it_ptr;
        }
    };   // fine const_iterator

    /**
     * Ritorna un iteratore a inizio set
     * @return iteratore all'inizio di un set
    */
    const_iterator begin() const {
      return const_iterator(ptr);
    }

    /**
     * Ritorna un iteratore a fine set
     * @return iteratore alla fine del set 
    */
    const_iterator end() const {
      return const_iterator(ptr + size);
    }
};

/**
 * Ritorna tutti gli elementi del set che soddisfano il predicato.
 * 
 * @param set set da filtrare
 * @param predicate predicato che un elemento deve soddisfare
 * 
 * @return set contenente gli elementi che soddisfano il predicato
*/
template<typename T, typename P>
Set<T> filter_out(const Set<T> &set, P predicate) {
  Set<T> filtered_set;
  for (typename Set<T>::const_iterator curr_itr = set.begin(); curr_itr != set.end(); ++curr_itr) {
    if (predicate(*curr_itr)) {
      filtered_set.add(*curr_itr);
    }
  }
  return filtered_set;
}

/**
 * Effettua la concatenazione di due set.
 * 
 * @param set1 Primo set
 * @param set2 Secondo set
 * 
 * @return set contenente gli elementi di entrambi i set
*/
template<typename T>
Set<T> operator+(const Set<T> &set1, const Set<T> &set2) {
  Set<T> concatenation(set1);
  for (typename Set<T>::const_iterator curr_itr = set2.begin(); curr_itr != set2.end(); ++curr_itr) {
    concatenation.add(*curr_itr);
  }
  return concatenation;
}

/**
 * Effettua l'intersezione di due set.
 * 
 * @param set1 Primo set
 * @param set2 Secondo set
 * 
 * @return set contenente gli elementi comuni a entrambi i set
*/
template<typename T>
Set<T> operator-(const Set<T> &set1, const Set<T> &set2) {
  Set<T> intersection;
  for (typename Set<T>::const_iterator curr_itr = set1.begin(); curr_itr != set1.end(); ++curr_itr) {
    if (set2.contains(*curr_itr)) {
      intersection.add(*curr_itr);
    }
  }
  return intersection;
}


/**
 * Salva un set di stringhe nel file specificato
 * 
 * @param set set di stringhe che si vuole salvare
 * @param filename nome del file in cui si vuole salvare
*/
// inline serve per evitare errori di linking
inline void save(Set<std::string> set, std::string filename) {
  // non serve un blocco try-catch perchè qualsiasi file aperto viene automaticamente
  // chiuso se si verifica un'eccezione, in quanto l'oggetto stream viene distrutto (da documentazione)
  std::fstream fs;
  fs.open(filename, std::fstream::out | std::fstream::app);
  fs << set;
  fs.close();
}
#endif  // SRC_SET_HPP_
