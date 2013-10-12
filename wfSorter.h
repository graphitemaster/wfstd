#include "wfAlgorithm.h"
#include "wfFunctional.h"

/*
 * Type: wfSortSelector
 * Describes the selection for the type of sorting to be performed
 * as the second argument to wfSorter
 *
 * Values:
 *   kSortType_Insertion -- Select insertion sort to be used by the sorter
 *   kSortType_Selection -- Select selection sort to be used by the sorter
 */
enum wfSortSelector {
	kSortType_Insertion,
	kSortType_Selection
};

namespace wfPrivate {
	template <typename T, typename V, wfSortSelector U>
	struct wfSortSystem;
	
	//
	// standard insertion sort for all containers uses iterators
	// works as expected
	//
	template <typename T, typename V>
	struct wfSortSystem<T, V, kSortType_Insertion> {
	private:
		typedef typename T::Iterator                           Iterator;
		typedef typename T::ConstIterator                      ConstIterator;
		typedef typename wfIteratorTraits<Iterator>::ValueType ValueType;
	
	public:
		void operator()(Iterator beg, Iterator end) {
			Iterator mover = beg;
			
			//
			// past one, mover points to the element being
			// inserted
			//
			mover++;
			
			while (mover != end) {
				ValueType value   = *mover;
				Iterator  current = mover;
				
				while (current != beg) {
					Iterator prev = current;
					
					//
					// past one, prev points to the location
					// before current
					//
					prev--;
					
					if (V()(*prev, value)) {
						*current = *prev;
						 current--;
					} else {
						break;
					}
				}
				
				*current = value;
				mover++;
			}
		}
	};
	
	//
	// standard selection sort for all containers, uses iterators
	// works as expected
	//
	template <typename T, typename V>
	struct wfSortSystem<T, V, kSortType_Selection> {
	private:
		typedef typename T::Iterator                           Iterator;
		typedef typename T::ConstIterator                      ConstIterator;
		typedef typename wfIteratorTraits<Iterator>::ValueType ValueType;
		
		Iterator Selection(Iterator beg, Iterator end) {
			Iterator lowest = beg;
			if (beg == end)
				return end;
				
			while (++beg != end) {
				if (V()(*beg, *end))
					lowest = beg;
			}
			
			return lowest;
		}
	public:
		void operator()(Iterator beg, Iterator end) {
			for (Iterator i = beg; i != end; ++i) {
				wfIteratorSwap<
					// take advantage of quicker swap
					// for POD types
					wfIsPOD<ValueType>::value
				>::Swap <
					Iterator,
					Iterator
				> (
					i,
					Selection (
						i,
						end
					)
				);
			}
		}
	};
}

template<typename T,  typename V, wfSortSelector U>
struct wfSorter : public wfPrivate::wfSortSystem<T, V, U> {
	//
	// should inhert operator ()
	//
};