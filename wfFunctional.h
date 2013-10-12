#ifndef WF_STDLIB_FUNCTIONAL_HDR
#define WF_STDLIB_FUNCTIONAL_HDR
#include "wfStandard.h"
#include "wfTypeTraits.h"

/* File: wfFunctional */
namespace wfPrivate {
	// == Functor
	template <bool>
	struct wfFunctionalEqual {
		template <typename U, typename V>
		static bool Execute(U lhs, V rhs) {
			return (lhs == rhs);
		}
		static bool Execute(const char *lhs, const char *rhs) {
			return (!lhs || !rhs) ? (lhs == rhs) : !strcmp(lhs, rhs);
		}
	};
	// != Functor
	template <bool>
	struct wfFunctionalNotEqual {
		template <typename U, typename V>
		static bool Execute(U lhs, V rhs) {
			return (lhs != rhs);
		}
		static bool Execute(const char *lhs, const char *rhs) {
			return (!lhs || !rhs) ? (lhs != rhs) : strcmp(lhs, rhs);
		}
	};
	// > Functor
	template <bool>
	struct wfFunctionalGreaterThan {
		template <typename U, typename V>
		static bool Execute(U lhs, V rhs) {
			return (lhs > rhs);
		}
		static bool Execute(const char *lhs, const char *rhs) {
			return (!lhs || !rhs) ? (lhs > rhs) : (strcmp(lhs, rhs) > 0);
		}
	};
	// < Functor
	template <bool>
	struct wfFunctionalLessThan {
		template <typename U, typename V>
		static bool Execute(U lhs, V rhs) {
			return (lhs < rhs);
		}
		static bool Execute(const char *lhs, const char *rhs) {
			return (!lhs || !rhs) ? (lhs < rhs) : (strcmp(lhs, rhs) < 0);
		}
	};
	// <= Functor
	template <bool>
	struct wfFunctionalLessThanOrEqual {
		template <typename U, typename V>
		static bool Execute(U lhs, V rhs) {
			return (lhs <= rhs);
		}
		static bool Execute(const char *lhs, const char *rhs) {
			return (!lhs || !rhs) ? (lhs <= rhs) : (strcmp(lhs, rhs) <= 0);
		}
	};
	// >= Functor
	template <bool>
	struct wfFunctionalGreaterThanOrEqual {
		template <typename U, typename V>
		static bool Execute(U lhs, V rhs) {
			return (lhs >= rhs);
		}
		static bool Execute(const char *lhs, const char *rhs) {
			return (!lhs || !rhs) ? (lhs >= rhs) : (strcmp(lhs, rhs) >= 0);
		}
	};

	//
	// Specializations for true variants (non POD variants)
	// which will not take the possible C string variants
	//
	template <> struct wfFunctionalEqual<true> {
		template <typename T, typename U>
		static bool Execute(const T& lhs, const U& rhs) { return (lhs == rhs); }
	};
	template <> struct wfFunctionalNotEqual<true> {
		template <typename T, typename U>
		static bool Execute(const T& lhs, const U& rhs) { return (lhs != rhs); }
	};
	template <> struct wfFunctionalGreaterThan<true> {
		template <typename T, typename U>
		static bool Execute(const T& lhs, const U& rhs) { return (lhs > rhs); }
	};
	template <> struct wfFunctionalLessThan<true> {
		template <typename T, typename U>
		static bool Execute(const T& lhs, const U& rhs) { return (lhs < rhs); }
	};
	template <> struct wfFunctionalGreaterThanOrEqual<true> {
		template <typename T, typename U>
		static bool Execute(const T& lhs, const U& rhs) { return (lhs >= rhs); }
	};
	template <> struct wfFunctionalLessThanOrEqual<true> {
		template <typename T, typename U>
		static bool Execute(const T& lhs, const U& rhs) { return (lhs <= rhs); }
	};


	template <typename T, typename U, typename V>
	struct wfBinaryFunction {
		typedef T Arg1Type;
		typedef U Arg2Type;
		typedef V ReturnValue;
	};
};

//
// namespaces to prevent collisions .. also it just looks weird when it's not in
// a namespace
//
namespace wfFunctional {
	/*
	 * Class: wfEqual
	 *  A binary predicate that tests whether a value of a specified
	 *  type is equal to another value of another type.
	 *
	 * Parameters:
	 *  lhs - The left operand of type *T* in the inequality to be tested.
	 *  rhs - The right operand of the type *U* in the inequality to be tested.
	 *
	 * Returns:
	 *  True if *lhs == rhs*; false otherwise.
	 */  
	template <typename T, typename U> struct wfEqual;
	template <typename T, typename U>
	struct wfEqual : wfPrivate::wfBinaryFunction<T, U, bool> {
		bool operator()(const T& lhs, const U& rhs) const {
			return wfPrivate::wfFunctionalEqual <
				!wfIsPOD<T>::value &&
				!wfIsPOD<U>::value
			>::Execute(lhs, rhs);
		}
	};

	/*
	 * Class: wfNotEqual
	 *  A binary predicate that tests whether a value of a specified
	 *  type is not equal to another value of another type.
	 *
	 * Parameters:
	 *  lhs - The left operand of type *T* in the equality to be tested.
	 *  rhs - The right operand of the type *U* in the equality to be tested.
	 *
	 * Returns:
	 *  True if *lhs* does not equal *rhs*, false otherwise.
	 */   
	template <typename T, typename U> struct wfNotEqual;
	template <typename T, typename U>
	struct wfNotEqual : wfPrivate::wfBinaryFunction<T, U, bool> {
		bool operator()(const T& lhs, const U& rhs) const {
			return wfPrivate::wfFunctionalNotEqual <
				!wfIsPOD<T>::value &&
				!wfIsPOD<U>::value
			>::Execute(lhs, rhs);
		}
	};

	/*
	 * Class: wfGreater
	 *  A binary predicate that tests whether a value of a specified
	 *  type is greater to another value of another type.
	 *
	 * Parameters:
	 *  lhs - The left operand of type *T* in the inequality to be tested.
	 *  rhs - The right operand of the type *U* in the inequality to be tested.
	 *
	 * Returns:
	 *  True if *lhs > rhs*; false otherwise.
	 */ 
	template <typename T, typename U> struct wfGreater;
	template <typename T, typename U>
	struct wfGreater : wfPrivate::wfBinaryFunction<T, U, bool> {
		bool operator()(const T& lhs, const U& rhs) const {
			return wfPrivate::wfFunctionalGreaterThan <
				!wfIsPOD<T>::value &&
				!wfIsPOD<U>::value
			>::Execute(lhs, rhs);
		}
	};

	/*
	 * Struct: wfLess
	 *  A binary predicate that tests whether a value of a specified
	 *  type is less to another value of another type.
	 *
	 * Parameters:
	 *  lhs - The left operand of type *T* in the inequality to be tested.
	 *  rhs - The right operand of the type *U* in the inequality to be tested.
	 *
	 * Returns:
	 *  True if *lhs < rhs*; false otherwise.
	 */ 
	template <typename T, typename U> struct wfLess;
	template <typename T, typename U>
	struct wfLess : wfPrivate::wfBinaryFunction<T, U, bool> {
		bool operator()(const T& lhs, const U& rhs) const {
			return wfPrivate::wfFunctionalLessThan <
				!wfIsPOD<T>::value &&
				!wfIsPOD<U>::value
			>::Execute(lhs, rhs);
		}
	};

	/*
	 * Struct: wfGreaterEqual
	 *  A binary predicate that tests whether a value of a specified
	 *  type is greater than or equal to another value of another type.
	 *
	 * Parameters:
	 *  lhs - The left operand of type *T* in the inequality to be tested.
	 *  rhs - The right operand of the type *U* in the inequality to be tested.
	 *
	 * Returns:
	 *  True if *lhs >= rhs*; false otherwise.
	 */    
	template <typename T, typename U> struct wfGreaterEqual;
	template <typename T, typename U>
	struct wfGreaterEqual : wfPrivate::wfBinaryFunction<T, U, bool> {
		bool operator()(const T& lhs, const U& rhs) const {
			return wfPrivate::wfFunctionalGreaterThanOrEqual <
				!wfIsPOD<T>::value &&
				!wfIsPOD<U>::value
			>::Execute(lhs, rhs);
		}
	};

	/*
	 * Struct: wfLessEqual
	 *  A binary predicate that tests whether a value of a specified
	 *  type is less than or equal to another value of another type.
	 *
	 * Parameters:
	 *  lhs - The left operand of type *T* in the inequality to be tested.
	 *  rhs - The right operand of the type *U* in the inequality to be tested.
	 *
	 * Returns:
	 *  True if *lhs <= rhs*; false otherwise.
	 */     
	template <typename T, typename U> struct wfLessEqual;
	template <typename T, typename U>
	struct wfLessEqual : wfPrivate::wfBinaryFunction<T, U, bool> {
		bool operator()(const T& lhs, const U& rhs) const {
			return wfPrivate::wfFunctionalLessThanOrEqual <
				!wfIsPOD<T>::value &&
				!wfIsPOD<U>::value
			>::Execute(lhs, rhs);
		}
	};
}
#endif
