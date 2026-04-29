#ifndef SORTED_LIST_H
#define SORTED_LIST_H
#include<list>
#include<vector>
#include<algorithm>
#include<functional>
// class SortedList<T, Cmp>
template<typename T, typename Cmp=std::less<T>>
class SortedList {

   private:
		  typedef std::list<T> data_t;

		  data_t data_;
		  //kulon mi van ha vectorral csinalnam
		  std::vector<T> data2_;
   public:
	 
	 typedef typename data_t::iterator iterator_t;
	 typedef typename data_t::const_iterator const_iterator_t;
    template<typename Iter>
	 SortedList(Iter begin, Iter end) : data_(begin,end) { 
		  data_.sort(Cmp());
		  std::sort(data2_.begin(),data2_.end(),Cmp());
	 }
	 SortedList() = default;
    void insert(const T& e) {

		  data_.push_back(e);
		  //data2_.push_back(e);
		  data_.sort(Cmp());
		  //std::sort(data2_.begin(),data2_.end(),Cmp());
		  //hatekonyabb insert vectorra rendezes szerint, e megfelelo pozicioja, megkapom a helyet,elorerendezeteken mukodik csak
		  iterator_t it =std::lower_bound(data2_.begin(),data2_.end(),e,Cmp());
		 
		  //beszurom a jo helyre
		  data2_.insert(it,e);
	 }

    void remove(const T& e) { 
		  iterator_t it = std::find(data_.begin(),data_.end(),e);
		  if(it != data_.end()) data_.erase(it);
		  

		  //vectorra jobb delete
		  std::pair<iterator_t,iterator_t> found =std::equal_range(data2_.begin(),data2_.end(),e,Cmp());
		  data2_.erase(found.first,found.second);
	 }


    int size() const { return data_.size();  }

    const T& front() const { 
		  return data_.front();
	 }
    const T& back() const { 
		  return data_.back();
	 }

    // iterators
	 iterator_t begin() { return data_.begin();}
	 iterator_t end() {return data_.end();}

	 const_iterator_t begin() const { return data_.begin();}
	 const_iterator_t end() const {return data_.end();}
};

#endif
