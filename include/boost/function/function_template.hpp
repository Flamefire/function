// Boost.Function library

//  Copyright Douglas Gregor 2001-2006
//  Copyright Emil Dotchevski 2007
//  Use, modification and distribution is subject to the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

// Note: this header is a header template and must NOT have multiple-inclusion
// protection.
#include <boost/function/detail/prologue.hpp>
#include <boost/core/no_exceptions_support.hpp>

#if defined(BOOST_MSVC)
#   pragma warning( push )
#   pragma warning( disable : 4127 ) // "conditional expression is constant"
#endif

#define BOOST_FUNCTION_TEMPLATE_PARMS typename... T
#define BOOST_FUNCTION_TEMPLATE_ARGS T...
#define BOOST_FUNCTION_PARMS T... a
#define BOOST_FUNCTION_ARGS static_cast<T&&>(a)...

// Always have commas (zero args case is handled with variadics too)
#define BOOST_FUNCTION_COMMA ,

// Class names used in this version of the code
#define BOOST_FUNCTION_FUNCTION_INVOKER             function_invoker
#define BOOST_FUNCTION_VOID_FUNCTION_INVOKER        void_function_invoker
#define BOOST_FUNCTION_FUNCTION_OBJ_INVOKER         function_obj_invoker
#define BOOST_FUNCTION_VOID_FUNCTION_OBJ_INVOKER    void_function_obj_invoker
#define BOOST_FUNCTION_FUNCTION_REF_INVOKER         function_ref_invoker
#define BOOST_FUNCTION_VOID_FUNCTION_REF_INVOKER    void_function_ref_invoker
#define BOOST_FUNCTION_MEMBER_INVOKER               function_mem_invoker
#define BOOST_FUNCTION_VOID_MEMBER_INVOKER          function_void_mem_invoker
#define BOOST_FUNCTION_GET_FUNCTION_INVOKER         get_function_invoker
#define BOOST_FUNCTION_GET_FUNCTION_OBJ_INVOKER     get_function_obj_invoker
#define BOOST_FUNCTION_GET_FUNCTION_REF_INVOKER     get_function_ref_invoker
#define BOOST_FUNCTION_GET_MEMBER_INVOKER           get_member_invoker
#define BOOST_FUNCTION_GET_INVOKER                  get_invoker
#define BOOST_FUNCTION_VTABLE                       basic_vtable

#define BOOST_FUNCTION_VOID_RETURN_TYPE void
#define BOOST_FUNCTION_RETURN(X) X

namespace boost {
  namespace detail {
    namespace function {
      template<
        typename FunctionPtr,
        typename R BOOST_FUNCTION_COMMA
        BOOST_FUNCTION_TEMPLATE_PARMS
        >
      struct BOOST_FUNCTION_FUNCTION_INVOKER
      {
        static R invoke(function_buffer& function_ptr BOOST_FUNCTION_COMMA
                        BOOST_FUNCTION_PARMS)
        {
          FunctionPtr f = reinterpret_cast<FunctionPtr>(function_ptr.members.func_ptr);
          return f(BOOST_FUNCTION_ARGS);
        }
      };

      template<
        typename FunctionPtr,
        typename R BOOST_FUNCTION_COMMA
        BOOST_FUNCTION_TEMPLATE_PARMS
        >
      struct BOOST_FUNCTION_VOID_FUNCTION_INVOKER
      {
        static BOOST_FUNCTION_VOID_RETURN_TYPE
        invoke(function_buffer& function_ptr BOOST_FUNCTION_COMMA
               BOOST_FUNCTION_PARMS)

        {
          FunctionPtr f = reinterpret_cast<FunctionPtr>(function_ptr.members.func_ptr);
          BOOST_FUNCTION_RETURN(f(BOOST_FUNCTION_ARGS));
        }
      };

      template<
        typename FunctionObj,
        typename R BOOST_FUNCTION_COMMA
        BOOST_FUNCTION_TEMPLATE_PARMS
      >
      struct BOOST_FUNCTION_FUNCTION_OBJ_INVOKER
      {
        static R invoke(function_buffer& function_obj_ptr BOOST_FUNCTION_COMMA
                        BOOST_FUNCTION_PARMS)

        {
          FunctionObj* f;
          if (function_allows_small_object_optimization<FunctionObj>::value)
            f = reinterpret_cast<FunctionObj*>(function_obj_ptr.data);
          else
            f = reinterpret_cast<FunctionObj*>(function_obj_ptr.members.obj_ptr);
          return (*f)(BOOST_FUNCTION_ARGS);
        }
      };

      template<
        typename FunctionObj,
        typename R BOOST_FUNCTION_COMMA
        BOOST_FUNCTION_TEMPLATE_PARMS
      >
      struct BOOST_FUNCTION_VOID_FUNCTION_OBJ_INVOKER
      {
        static BOOST_FUNCTION_VOID_RETURN_TYPE
        invoke(function_buffer& function_obj_ptr BOOST_FUNCTION_COMMA
               BOOST_FUNCTION_PARMS)

        {
          FunctionObj* f;
          if (function_allows_small_object_optimization<FunctionObj>::value)
            f = reinterpret_cast<FunctionObj*>(function_obj_ptr.data);
          else
            f = reinterpret_cast<FunctionObj*>(function_obj_ptr.members.obj_ptr);
          BOOST_FUNCTION_RETURN((*f)(BOOST_FUNCTION_ARGS));
        }
      };

      template<
        typename FunctionObj,
        typename R BOOST_FUNCTION_COMMA
        BOOST_FUNCTION_TEMPLATE_PARMS
      >
      struct BOOST_FUNCTION_FUNCTION_REF_INVOKER
      {
        static R invoke(function_buffer& function_obj_ptr BOOST_FUNCTION_COMMA
                        BOOST_FUNCTION_PARMS)

        {
          FunctionObj* f =
            reinterpret_cast<FunctionObj*>(function_obj_ptr.members.obj_ptr);
          return (*f)(BOOST_FUNCTION_ARGS);
        }
      };

      template<
        typename FunctionObj,
        typename R BOOST_FUNCTION_COMMA
        BOOST_FUNCTION_TEMPLATE_PARMS
      >
      struct BOOST_FUNCTION_VOID_FUNCTION_REF_INVOKER
      {
        static BOOST_FUNCTION_VOID_RETURN_TYPE
        invoke(function_buffer& function_obj_ptr BOOST_FUNCTION_COMMA
               BOOST_FUNCTION_PARMS)

        {
          FunctionObj* f =
            reinterpret_cast<FunctionObj*>(function_obj_ptr.members.obj_ptr);
          BOOST_FUNCTION_RETURN((*f)(BOOST_FUNCTION_ARGS));
        }
      };

#if BOOST_FUNCTION_NUM_ARGS > 0
      /* Handle invocation of member pointers. */
      template<
        typename MemberPtr,
        typename R BOOST_FUNCTION_COMMA
        BOOST_FUNCTION_TEMPLATE_PARMS
      >
      struct BOOST_FUNCTION_MEMBER_INVOKER
      {
        static R invoke(function_buffer& function_obj_ptr BOOST_FUNCTION_COMMA
                        BOOST_FUNCTION_PARMS)

        {
          MemberPtr* f =
            reinterpret_cast<MemberPtr*>(function_obj_ptr.data);
          return boost::mem_fn(*f)(BOOST_FUNCTION_ARGS);
        }
      };

      template<
        typename MemberPtr,
        typename R BOOST_FUNCTION_COMMA
        BOOST_FUNCTION_TEMPLATE_PARMS
      >
      struct BOOST_FUNCTION_VOID_MEMBER_INVOKER
      {
        static BOOST_FUNCTION_VOID_RETURN_TYPE
        invoke(function_buffer& function_obj_ptr BOOST_FUNCTION_COMMA
               BOOST_FUNCTION_PARMS)

        {
          MemberPtr* f =
            reinterpret_cast<MemberPtr*>(function_obj_ptr.data);
          BOOST_FUNCTION_RETURN(boost::mem_fn(*f)(BOOST_FUNCTION_ARGS));
        }
      };
#endif

      template<
        typename FunctionPtr,
        typename R BOOST_FUNCTION_COMMA
        BOOST_FUNCTION_TEMPLATE_PARMS
      >
      struct BOOST_FUNCTION_GET_FUNCTION_INVOKER
      {
        typedef typename conditional<(is_void<R>::value),
                            BOOST_FUNCTION_VOID_FUNCTION_INVOKER<
                            FunctionPtr,
                            R BOOST_FUNCTION_COMMA
                            BOOST_FUNCTION_TEMPLATE_ARGS
                          >,
                          BOOST_FUNCTION_FUNCTION_INVOKER<
                            FunctionPtr,
                            R BOOST_FUNCTION_COMMA
                            BOOST_FUNCTION_TEMPLATE_ARGS
                          >
                       >::type type;
      };

      template<
        typename FunctionObj,
        typename R BOOST_FUNCTION_COMMA
        BOOST_FUNCTION_TEMPLATE_PARMS
       >
      struct BOOST_FUNCTION_GET_FUNCTION_OBJ_INVOKER
      {
        typedef typename conditional<(is_void<R>::value),
                            BOOST_FUNCTION_VOID_FUNCTION_OBJ_INVOKER<
                            FunctionObj,
                            R BOOST_FUNCTION_COMMA
                            BOOST_FUNCTION_TEMPLATE_ARGS
                          >,
                          BOOST_FUNCTION_FUNCTION_OBJ_INVOKER<
                            FunctionObj,
                            R BOOST_FUNCTION_COMMA
                            BOOST_FUNCTION_TEMPLATE_ARGS
                          >
                       >::type type;
      };

      template<
        typename FunctionObj,
        typename R BOOST_FUNCTION_COMMA
        BOOST_FUNCTION_TEMPLATE_PARMS
       >
      struct BOOST_FUNCTION_GET_FUNCTION_REF_INVOKER
      {
        typedef typename conditional<(is_void<R>::value),
                            BOOST_FUNCTION_VOID_FUNCTION_REF_INVOKER<
                            FunctionObj,
                            R BOOST_FUNCTION_COMMA
                            BOOST_FUNCTION_TEMPLATE_ARGS
                          >,
                          BOOST_FUNCTION_FUNCTION_REF_INVOKER<
                            FunctionObj,
                            R BOOST_FUNCTION_COMMA
                            BOOST_FUNCTION_TEMPLATE_ARGS
                          >
                       >::type type;
      };

#if BOOST_FUNCTION_NUM_ARGS > 0
      /* Retrieve the appropriate invoker for a member pointer.  */
      template<
        typename MemberPtr,
        typename R BOOST_FUNCTION_COMMA
        BOOST_FUNCTION_TEMPLATE_PARMS
       >
      struct BOOST_FUNCTION_GET_MEMBER_INVOKER
      {
        typedef typename conditional<(is_void<R>::value),
                            BOOST_FUNCTION_VOID_MEMBER_INVOKER<
                            MemberPtr,
                            R BOOST_FUNCTION_COMMA
                            BOOST_FUNCTION_TEMPLATE_ARGS
                          >,
                          BOOST_FUNCTION_MEMBER_INVOKER<
                            MemberPtr,
                            R BOOST_FUNCTION_COMMA
                            BOOST_FUNCTION_TEMPLATE_ARGS
                          >
                       >::type type;
      };
#endif

      /* Given the tag returned by get_function_tag, retrieve the
         actual invoker that will be used for the given function
         object.

         Each specialization contains an "apply" nested class template
         that accepts the function object, return type, function
         argument types, and allocator. The resulting "apply" class
         contains two typedefs, "invoker_type" and "manager_type",
         which correspond to the invoker and manager types. */
      template<typename Tag>
      struct BOOST_FUNCTION_GET_INVOKER { };

      /* Retrieve the invoker for a function pointer. */
      template<>
      struct BOOST_FUNCTION_GET_INVOKER<function_ptr_tag>
      {
        template<typename FunctionPtr,
                 typename R BOOST_FUNCTION_COMMA BOOST_FUNCTION_TEMPLATE_PARMS>
        struct apply
        {
          typedef typename BOOST_FUNCTION_GET_FUNCTION_INVOKER<
                             FunctionPtr,
                             R BOOST_FUNCTION_COMMA
                             BOOST_FUNCTION_TEMPLATE_ARGS
                           >::type
            invoker_type;

          typedef functor_manager<FunctionPtr> manager_type;
        };

        template<typename FunctionPtr, typename Allocator,
                 typename R BOOST_FUNCTION_COMMA BOOST_FUNCTION_TEMPLATE_PARMS>
        struct apply_a
        {
          typedef typename BOOST_FUNCTION_GET_FUNCTION_INVOKER<
                             FunctionPtr,
                             R BOOST_FUNCTION_COMMA
                             BOOST_FUNCTION_TEMPLATE_ARGS
                           >::type
            invoker_type;

          typedef functor_manager<FunctionPtr> manager_type;
        };
      };

#if BOOST_FUNCTION_NUM_ARGS > 0
      /* Retrieve the invoker for a member pointer. */
      template<>
      struct BOOST_FUNCTION_GET_INVOKER<member_ptr_tag>
      {
        template<typename MemberPtr,
                 typename R BOOST_FUNCTION_COMMA BOOST_FUNCTION_TEMPLATE_PARMS>
        struct apply
        {
          typedef typename BOOST_FUNCTION_GET_MEMBER_INVOKER<
                             MemberPtr,
                             R BOOST_FUNCTION_COMMA
                             BOOST_FUNCTION_TEMPLATE_ARGS
                           >::type
            invoker_type;

          typedef functor_manager<MemberPtr> manager_type;
        };

        template<typename MemberPtr, typename Allocator,
                 typename R BOOST_FUNCTION_COMMA BOOST_FUNCTION_TEMPLATE_PARMS>
        struct apply_a
        {
          typedef typename BOOST_FUNCTION_GET_MEMBER_INVOKER<
                             MemberPtr,
                             R BOOST_FUNCTION_COMMA
                             BOOST_FUNCTION_TEMPLATE_ARGS
                           >::type
            invoker_type;

          typedef functor_manager<MemberPtr> manager_type;
        };
      };
#endif

      /* Retrieve the invoker for a function object. */
      template<>
      struct BOOST_FUNCTION_GET_INVOKER<function_obj_tag>
      {
        template<typename FunctionObj,
                 typename R BOOST_FUNCTION_COMMA BOOST_FUNCTION_TEMPLATE_PARMS>
        struct apply
        {
          typedef typename BOOST_FUNCTION_GET_FUNCTION_OBJ_INVOKER<
                             FunctionObj,
                             R BOOST_FUNCTION_COMMA
                             BOOST_FUNCTION_TEMPLATE_ARGS
                           >::type
            invoker_type;

          typedef functor_manager<FunctionObj> manager_type;
        };

        template<typename FunctionObj, typename Allocator,
                 typename R BOOST_FUNCTION_COMMA BOOST_FUNCTION_TEMPLATE_PARMS>
        struct apply_a
        {
          typedef typename BOOST_FUNCTION_GET_FUNCTION_OBJ_INVOKER<
                             FunctionObj,
                             R BOOST_FUNCTION_COMMA
                             BOOST_FUNCTION_TEMPLATE_ARGS
                           >::type
            invoker_type;

          typedef functor_manager_a<FunctionObj, Allocator> manager_type;
        };
      };

      /* Retrieve the invoker for a reference to a function object. */
      template<>
      struct BOOST_FUNCTION_GET_INVOKER<function_obj_ref_tag>
      {
        template<typename RefWrapper,
                 typename R BOOST_FUNCTION_COMMA BOOST_FUNCTION_TEMPLATE_PARMS>
        struct apply
        {
          typedef typename BOOST_FUNCTION_GET_FUNCTION_REF_INVOKER<
                             typename RefWrapper::type,
                             R BOOST_FUNCTION_COMMA
                             BOOST_FUNCTION_TEMPLATE_ARGS
                           >::type
            invoker_type;

          typedef reference_manager<typename RefWrapper::type> manager_type;
        };

        template<typename RefWrapper, typename Allocator,
                 typename R BOOST_FUNCTION_COMMA BOOST_FUNCTION_TEMPLATE_PARMS>
        struct apply_a
        {
          typedef typename BOOST_FUNCTION_GET_FUNCTION_REF_INVOKER<
                             typename RefWrapper::type,
                             R BOOST_FUNCTION_COMMA
                             BOOST_FUNCTION_TEMPLATE_ARGS
                           >::type
            invoker_type;

          typedef reference_manager<typename RefWrapper::type> manager_type;
        };
      };


      /**
       * vtable for a specific boost::function instance. This
       * structure must be an aggregate so that we can use static
       * initialization in boost::function's assign_to and assign_to_a
       * members. It therefore cannot have any constructors,
       * destructors, base classes, etc.
       */
      template<typename R BOOST_FUNCTION_COMMA BOOST_FUNCTION_TEMPLATE_PARMS>
      struct BOOST_FUNCTION_VTABLE
      {
        typedef R         result_type;

        typedef result_type (*invoker_type)(function_buffer&
                                            BOOST_FUNCTION_COMMA
                                            BOOST_FUNCTION_TEMPLATE_ARGS);

        template<typename F>
        bool assign_to(F f, function_buffer& functor) const
        {
          typedef typename get_function_tag<F>::type tag;
          return assign_to(f, functor, tag());
        }
        template<typename F,typename Allocator>
        bool assign_to_a(F f, function_buffer& functor, Allocator a) const
        {
          typedef typename get_function_tag<F>::type tag;
          return assign_to_a(f, functor, a, tag());
        }

        void clear(function_buffer& functor) const
        {
#if defined(BOOST_GCC) && (__GNUC__ >= 11)
# pragma GCC diagnostic push
// False positive in GCC 11/12 for empty function objects
# pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif
          if (base.manager)
            base.manager(functor, functor, destroy_functor_tag);
#if defined(BOOST_GCC) && (__GNUC__ >= 11)
# pragma GCC diagnostic pop
#endif
        }

      private:
        // Function pointers
        template<typename FunctionPtr>
        bool
        assign_to(FunctionPtr f, function_buffer& functor, function_ptr_tag) const
        {
          this->clear(functor);
          if (f) {
            // should be a reinterpret cast, but some compilers insist
            // on giving cv-qualifiers to free functions
            functor.members.func_ptr = reinterpret_cast<void (*)()>(f);
            return true;
          } else {
            return false;
          }
        }
        template<typename FunctionPtr,typename Allocator>
        bool
        assign_to_a(FunctionPtr f, function_buffer& functor, Allocator, function_ptr_tag) const
        {
          return assign_to(f,functor,function_ptr_tag());
        }

        // Member pointers
#if BOOST_FUNCTION_NUM_ARGS > 0
        template<typename MemberPtr>
        bool assign_to(MemberPtr f, function_buffer& functor, member_ptr_tag) const
        {
          // DPG TBD: Add explicit support for member function
          // objects, so we invoke through mem_fn() but we retain the
          // right target_type() values.
          if (f) {
            this->assign_to(boost::mem_fn(f), functor);
            return true;
          } else {
            return false;
          }
        }
        template<typename MemberPtr,typename Allocator>
        bool assign_to_a(MemberPtr f, function_buffer& functor, Allocator a, member_ptr_tag) const
        {
          // DPG TBD: Add explicit support for member function
          // objects, so we invoke through mem_fn() but we retain the
          // right target_type() values.
          if (f) {
            this->assign_to_a(boost::mem_fn(f), functor, a);
            return true;
          } else {
            return false;
          }
        }
#endif // BOOST_FUNCTION_NUM_ARGS > 0

        // Function objects
        // Assign to a function object using the small object optimization
        template<typename FunctionObj>
        void
        assign_functor(FunctionObj f, function_buffer& functor, true_type) const
        {
          new (reinterpret_cast<void*>(functor.data)) FunctionObj(f);
        }
        template<typename FunctionObj,typename Allocator>
        void
        assign_functor_a(FunctionObj f, function_buffer& functor, Allocator, true_type) const
        {
          assign_functor(f,functor,true_type());
        }

        // Assign to a function object allocated on the heap.
        template<typename FunctionObj>
        void
        assign_functor(FunctionObj f, function_buffer& functor, false_type) const
        {
          functor.members.obj_ptr = new FunctionObj(f);
        }
        template<typename FunctionObj,typename Allocator>
        void
        assign_functor_a(FunctionObj f, function_buffer& functor, Allocator a, false_type) const
        {
          typedef functor_wrapper<FunctionObj,Allocator> functor_wrapper_type;

          using wrapper_allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<functor_wrapper_type>;
          using wrapper_allocator_pointer_type = typename std::allocator_traits<wrapper_allocator_type>::pointer;

          wrapper_allocator_type wrapper_allocator(a);
          wrapper_allocator_pointer_type copy = wrapper_allocator.allocate(1);
          std::allocator_traits<wrapper_allocator_type>::construct(wrapper_allocator, copy, functor_wrapper_type(f,a));

          functor_wrapper_type* new_f = static_cast<functor_wrapper_type*>(copy);
          functor.members.obj_ptr = new_f;
        }

        template<typename FunctionObj>
        bool
        assign_to(FunctionObj f, function_buffer& functor, function_obj_tag) const
        {
          if (!boost::detail::function::has_empty_target(boost::addressof(f))) {
            assign_functor(f, functor,
                           integral_constant<bool, (function_allows_small_object_optimization<FunctionObj>::value)>());
            return true;
          } else {
            return false;
          }
        }
        template<typename FunctionObj,typename Allocator>
        bool
        assign_to_a(FunctionObj f, function_buffer& functor, Allocator a, function_obj_tag) const
        {
          if (!boost::detail::function::has_empty_target(boost::addressof(f))) {
            assign_functor_a(f, functor, a,
                           integral_constant<bool, (function_allows_small_object_optimization<FunctionObj>::value)>());
            return true;
          } else {
            return false;
          }
        }

        // Reference to a function object
        template<typename FunctionObj>
        bool
        assign_to(const reference_wrapper<FunctionObj>& f,
                  function_buffer& functor, function_obj_ref_tag) const
        {
          functor.members.obj_ref.obj_ptr = (void *)(f.get_pointer());
          functor.members.obj_ref.is_const_qualified = is_const<FunctionObj>::value;
          functor.members.obj_ref.is_volatile_qualified = is_volatile<FunctionObj>::value;
          return true;
        }
        template<typename FunctionObj,typename Allocator>
        bool
        assign_to_a(const reference_wrapper<FunctionObj>& f,
                  function_buffer& functor, Allocator, function_obj_ref_tag) const
        {
          return assign_to(f,functor,function_obj_ref_tag());
        }

      public:
        vtable_base base;
        invoker_type invoker;
      };

      template <typename... T>
      struct variadic_function_base
      {};

      template <typename T>
      struct variadic_function_base<T>
      {
        typedef T argument_type;
      };

      template <typename T0, typename T1>
      struct variadic_function_base<T0, T1>
      {
        typedef T0 first_argument_type;
        typedef T1 second_argument_type;
      };

    } // end namespace function
  } // end namespace detail

  template<
    typename R BOOST_FUNCTION_COMMA
    BOOST_FUNCTION_TEMPLATE_PARMS
  >
  class function_n : public function_base
                                , public detail::function::variadic_function_base<T...>
  {
  public:
    typedef R         result_type;

  private:
    typedef boost::detail::function::BOOST_FUNCTION_VTABLE<
              R BOOST_FUNCTION_COMMA BOOST_FUNCTION_TEMPLATE_ARGS>
      vtable_type;

    vtable_type* get_vtable() const {
      return reinterpret_cast<vtable_type*>(
               reinterpret_cast<std::size_t>(vtable) & ~static_cast<std::size_t>(0x01));
    }

    struct clear_type {};

  public:
    // add signature for boost::lambda
    template<typename Args>
    struct sig
    {
      typedef result_type type;
    };

    BOOST_STATIC_CONSTANT(int, arity = sizeof...(T));

    typedef function_n self_type;

    BOOST_DEFAULTED_FUNCTION(function_n(), : function_base() {})

    // MSVC chokes if the following two constructors are collapsed into
    // one with a default parameter.
    template<typename Functor>
    function_n(Functor f
                            ,typename boost::enable_if_<
                             !(is_integral<Functor>::value),
                                        int>::type = 0
                            ) :
      function_base()
    {
      this->assign_to(f);
    }
    template<typename Functor,typename Allocator>
    function_n(Functor f, Allocator a
                            ,typename boost::enable_if_<
                              !(is_integral<Functor>::value),
                                        int>::type = 0
                            ) :
      function_base()
    {
      this->assign_to_a(f,a);
    }

    function_n(clear_type*) : function_base() { }

    function_n(const function_n& f) : function_base()
    {
      this->assign_to_own(f);
    }

    function_n(function_n&& f) : function_base()
    {
      this->move_assign(f);
    }

    ~function_n() { clear(); }

    result_type operator()(BOOST_FUNCTION_PARMS) const
    {
      if (this->empty())
        boost::throw_exception(bad_function_call());

      return get_vtable()->invoker
               (this->functor BOOST_FUNCTION_COMMA BOOST_FUNCTION_ARGS);
    }

    // The distinction between when to use function_n and
    // when to use self_type is obnoxious. MSVC cannot handle self_type as
    // the return type of these assignment operators, but Borland C++ cannot
    // handle function_n as the type of the temporary to
    // construct.
    template<typename Functor>
    typename boost::enable_if_<
                  !(is_integral<Functor>::value),
               function_n&>::type
    operator=(Functor f)
    {
      this->clear();
      BOOST_TRY  {
        this->assign_to(f);
      } BOOST_CATCH (...) {
        vtable = 0;
        BOOST_RETHROW;
      }
      BOOST_CATCH_END
      return *this;
    }
    template<typename Functor,typename Allocator>
    void assign(Functor f, Allocator a)
    {
      this->clear();
      BOOST_TRY{
        this->assign_to_a(f,a);
      } BOOST_CATCH (...) {
        vtable = 0;
        BOOST_RETHROW;
      }
      BOOST_CATCH_END
    }

    function_n& operator=(clear_type*)
    {
      this->clear();
      return *this;
    }

    // Assignment from another function_n
    function_n& operator=(const function_n& f)
    {
      if (&f == this)
        return *this;

      this->clear();
      BOOST_TRY {
        this->assign_to_own(f);
      } BOOST_CATCH (...) {
        vtable = 0;
        BOOST_RETHROW;
      }
      BOOST_CATCH_END
      return *this;
    }

    // Move assignment from another function_n
    function_n& operator=(function_n&& f)
    {
      if (&f == this)
        return *this;

      this->clear();
      BOOST_TRY {
        this->move_assign(f);
      } BOOST_CATCH (...) {
        vtable = 0;
        BOOST_RETHROW;
      }
      BOOST_CATCH_END
      return *this;
    }

    void swap(function_n& other)
    {
      if (&other == this)
        return;

      function_n tmp;
      tmp.move_assign(*this);
      this->move_assign(other);
      other.move_assign(tmp);
    }

    // Clear out a target, if there is one
    void clear()
    {
      if (vtable) {
        if (!this->has_trivial_copy_and_destroy())
          get_vtable()->clear(this->functor);
        vtable = 0;
      }
    }

    explicit operator bool () const { return !this->empty(); }

  private:
    void assign_to_own(const function_n& f)
    {
      if (!f.empty()) {
        this->vtable = f.vtable;
        if (this->has_trivial_copy_and_destroy()) {
          // Don't operate on storage directly since union type doesn't relax
          // strict aliasing rules, despite of having member char type.
#         if defined(BOOST_GCC) && (BOOST_GCC >= 40700)
#           pragma GCC diagnostic push
            // This warning is technically correct, but we don't want to pay the price for initializing
            // just to silence a warning: https://github.com/boostorg/function/issues/27
#           pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#           if (BOOST_GCC >= 110000)
              // GCC 11.3, 12 emit a different warning: https://github.com/boostorg/function/issues/42
#             pragma GCC diagnostic ignored "-Wuninitialized"
#           endif
#         endif
          std::memcpy(this->functor.data, f.functor.data, sizeof(boost::detail::function::function_buffer));
#         if defined(BOOST_GCC) && (BOOST_GCC >= 40700)
#           pragma GCC diagnostic pop
#         endif
        } else
          get_vtable()->base.manager(f.functor, this->functor,
                                     boost::detail::function::clone_functor_tag);
      }
    }

    template<typename Functor>
    void assign_to(Functor f)
    {
      using boost::detail::function::vtable_base;

      typedef typename boost::detail::function::get_function_tag<Functor>::type tag;
      typedef boost::detail::function::BOOST_FUNCTION_GET_INVOKER<tag> get_invoker;
      typedef typename get_invoker::
                         template apply<Functor, R BOOST_FUNCTION_COMMA
                        BOOST_FUNCTION_TEMPLATE_ARGS>
        handler_type;

      typedef typename handler_type::invoker_type invoker_type;
      typedef typename handler_type::manager_type manager_type;

      // Note: it is extremely important that this initialization use
      // static initialization. Otherwise, we will have a race
      // condition here in multi-threaded code. See
      // http://thread.gmane.org/gmane.comp.lib.boost.devel/164902/.
      static const vtable_type stored_vtable =
        { { &manager_type::manage }, &invoker_type::invoke };

      if (stored_vtable.assign_to(f, functor)) {
        std::size_t value = reinterpret_cast<std::size_t>(&stored_vtable.base);
        // coverity[pointless_expression]: suppress coverity warnings on apparant if(const).
        if (boost::has_trivial_copy_constructor<Functor>::value &&
            boost::has_trivial_destructor<Functor>::value &&
            boost::detail::function::function_allows_small_object_optimization<Functor>::value)
          value |= static_cast<std::size_t>(0x01);
        vtable = reinterpret_cast<boost::detail::function::vtable_base *>(value);
      } else
        vtable = 0;
    }

    template<typename Functor,typename Allocator>
    void assign_to_a(Functor f,Allocator a)
    {
      using boost::detail::function::vtable_base;

      typedef typename boost::detail::function::get_function_tag<Functor>::type tag;
      typedef boost::detail::function::BOOST_FUNCTION_GET_INVOKER<tag> get_invoker;
      typedef typename get_invoker::
                         template apply_a<Functor, Allocator, R BOOST_FUNCTION_COMMA
                         BOOST_FUNCTION_TEMPLATE_ARGS>
        handler_type;

      typedef typename handler_type::invoker_type invoker_type;
      typedef typename handler_type::manager_type manager_type;

      // Note: it is extremely important that this initialization use
      // static initialization. Otherwise, we will have a race
      // condition here in multi-threaded code. See
      // http://thread.gmane.org/gmane.comp.lib.boost.devel/164902/.
      static const vtable_type stored_vtable =
        { { &manager_type::manage }, &invoker_type::invoke };

      if (stored_vtable.assign_to_a(f, functor, a)) {
        std::size_t value = reinterpret_cast<std::size_t>(&stored_vtable.base);
        // coverity[pointless_expression]: suppress coverity warnings on apparant if(const).
        if (boost::has_trivial_copy_constructor<Functor>::value &&
            boost::has_trivial_destructor<Functor>::value &&
            boost::detail::function::function_allows_small_object_optimization<Functor>::value)
          value |= static_cast<std::size_t>(0x01);
        vtable = reinterpret_cast<boost::detail::function::vtable_base *>(value);
      } else
        vtable = 0;
    }

    // Moves the value from the specified argument to *this. If the argument
    // has its function object allocated on the heap, move_assign will pass
    // its buffer to *this, and set the argument's buffer pointer to NULL.
    void move_assign(function_n& f)
    {
      if (&f == this)
        return;

      BOOST_TRY {
        if (!f.empty()) {
          this->vtable = f.vtable;
          if (this->has_trivial_copy_and_destroy()) {
            // Don't operate on storage directly since union type doesn't relax
            // strict aliasing rules, despite of having member char type.
#           if defined(BOOST_GCC) && (BOOST_GCC >= 40700)
#             pragma GCC diagnostic push
              // This warning is technically correct, but we don't want to pay the price for initializing
              // just to silence a warning: https://github.com/boostorg/function/issues/27
#             pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#             if (BOOST_GCC >= 120000)
                // GCC 12 emits a different warning: https://github.com/boostorg/function/issues/42
#               pragma GCC diagnostic ignored "-Wuninitialized"
#             endif
#           endif
            std::memcpy(this->functor.data, f.functor.data, sizeof(this->functor.data));
#           if defined(BOOST_GCC) && (BOOST_GCC >= 40700)
#             pragma GCC diagnostic pop
#           endif
          } else
#if defined(BOOST_GCC) && (__GNUC__ >= 11)
# pragma GCC diagnostic push
// False positive in GCC 11/12 for empty function objects (function_n_test.cpp:673)
# pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif
            get_vtable()->base.manager(f.functor, this->functor,
                                     boost::detail::function::move_functor_tag);
#if defined(BOOST_GCC) && (__GNUC__ >= 11)
# pragma GCC diagnostic pop
#endif
          f.vtable = 0;
        } else {
          clear();
        }
      } BOOST_CATCH (...) {
        vtable = 0;
        BOOST_RETHROW;
      }
      BOOST_CATCH_END
    }
  };

  template<typename R BOOST_FUNCTION_COMMA BOOST_FUNCTION_TEMPLATE_PARMS>
  inline void swap(function_n<
                     R BOOST_FUNCTION_COMMA
                     BOOST_FUNCTION_TEMPLATE_ARGS
                   >& f1,
                   function_n<
                     R BOOST_FUNCTION_COMMA
                     BOOST_FUNCTION_TEMPLATE_ARGS
                   >& f2)
  {
    f1.swap(f2);
  }

// Poison comparisons between boost::function objects of the same type.
template<typename R BOOST_FUNCTION_COMMA BOOST_FUNCTION_TEMPLATE_PARMS>
  void operator==(const function_n<
                          R BOOST_FUNCTION_COMMA
                          BOOST_FUNCTION_TEMPLATE_ARGS>&,
                  const function_n<
                          R BOOST_FUNCTION_COMMA
                          BOOST_FUNCTION_TEMPLATE_ARGS>&);
template<typename R BOOST_FUNCTION_COMMA BOOST_FUNCTION_TEMPLATE_PARMS>
  void operator!=(const function_n<
                          R BOOST_FUNCTION_COMMA
                          BOOST_FUNCTION_TEMPLATE_ARGS>&,
                  const function_n<
                          R BOOST_FUNCTION_COMMA
                          BOOST_FUNCTION_TEMPLATE_ARGS>& );

#define BOOST_FUNCTION_PARTIAL_SPEC R (BOOST_FUNCTION_TEMPLATE_ARGS)

template<typename R BOOST_FUNCTION_COMMA
         BOOST_FUNCTION_TEMPLATE_PARMS>
class function<BOOST_FUNCTION_PARTIAL_SPEC>
  : public function_n<R BOOST_FUNCTION_COMMA BOOST_FUNCTION_TEMPLATE_ARGS>
{
  typedef function_n<R BOOST_FUNCTION_COMMA BOOST_FUNCTION_TEMPLATE_ARGS> base_type;
  typedef function self_type;

  struct clear_type {};

public:

  BOOST_DEFAULTED_FUNCTION(function(), : base_type() {})

  template<typename Functor>
  function(Functor f
           ,typename boost::enable_if_<
                          !(is_integral<Functor>::value),
                       int>::type = 0
           ) :
    base_type(f)
  {
  }
  template<typename Functor,typename Allocator>
  function(Functor f, Allocator a
           ,typename boost::enable_if_<
                           !(is_integral<Functor>::value),
                       int>::type = 0
           ) :
    base_type(f,a)
  {
  }

  function(clear_type*) : base_type() {}

  function(const self_type& f) : base_type(static_cast<const base_type&>(f)){}

  function(const base_type& f) : base_type(static_cast<const base_type&>(f)){}

  // Move constructors
  function(self_type&& f): base_type(static_cast<base_type&&>(f)){}
  function(base_type&& f): base_type(static_cast<base_type&&>(f)){}

  self_type& operator=(const self_type& f)
  {
    self_type(f).swap(*this);
    return *this;
  }

  self_type& operator=(self_type&& f)
  {
    self_type(static_cast<self_type&&>(f)).swap(*this);
    return *this;
  }

  template<typename Functor>
  typename boost::enable_if_<
                         !(is_integral<Functor>::value),
                      self_type&>::type
  operator=(Functor f)
  {
    self_type(f).swap(*this);
    return *this;
  }

  self_type& operator=(clear_type*)
  {
    this->clear();
    return *this;
  }

  self_type& operator=(const base_type& f)
  {
    self_type(f).swap(*this);
    return *this;
  }

  self_type& operator=(base_type&& f)
  {
    self_type(static_cast<base_type&&>(f)).swap(*this);
    return *this;
  }
};

#undef BOOST_FUNCTION_PARTIAL_SPEC

} // end namespace boost

// Cleanup after ourselves...
#undef BOOST_FUNCTION_VTABLE
#undef BOOST_FUNCTION_COMMA
#undef BOOST_FUNCTION_FUNCTION_INVOKER
#undef BOOST_FUNCTION_VOID_FUNCTION_INVOKER
#undef BOOST_FUNCTION_FUNCTION_OBJ_INVOKER
#undef BOOST_FUNCTION_VOID_FUNCTION_OBJ_INVOKER
#undef BOOST_FUNCTION_FUNCTION_REF_INVOKER
#undef BOOST_FUNCTION_VOID_FUNCTION_REF_INVOKER
#undef BOOST_FUNCTION_MEMBER_INVOKER
#undef BOOST_FUNCTION_VOID_MEMBER_INVOKER
#undef BOOST_FUNCTION_GET_FUNCTION_INVOKER
#undef BOOST_FUNCTION_GET_FUNCTION_OBJ_INVOKER
#undef BOOST_FUNCTION_GET_FUNCTION_REF_INVOKER
#undef BOOST_FUNCTION_GET_MEM_FUNCTION_INVOKER
#undef BOOST_FUNCTION_GET_INVOKER
#undef BOOST_FUNCTION_TEMPLATE_PARMS
#undef BOOST_FUNCTION_TEMPLATE_ARGS
#undef BOOST_FUNCTION_PARMS
#undef BOOST_FUNCTION_PARM
#ifdef BOOST_FUNCTION_ARG
#   undef BOOST_FUNCTION_ARG
#endif
#undef BOOST_FUNCTION_ARGS
#undef BOOST_FUNCTION_ARG_TYPE
#undef BOOST_FUNCTION_ARG_TYPES
#undef BOOST_FUNCTION_VOID_RETURN_TYPE
#undef BOOST_FUNCTION_RETURN

#if defined(BOOST_MSVC)
#   pragma warning( pop )
#endif
