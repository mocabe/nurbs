#pragma once

// Tags
namespace nurbs{
  namespace tags{
    // buffer tags
    struct HeapArray;
    struct StackArray;
    // eval tags
    struct Recursive;
    template <class BufferTag = HeapArray> 
    struct NonRecursive;

    // still supports macros
    #if defined(NURBS_NON_RECURSIVE)
      #if defined(NURBS_ENABLE_STACK_ALLOCATION)
        using default_eval_tag = NonRecursive<StackArray>;
      #else
        using default_eval_tag = NonRecursive<HeapArray>;
      #endif
    #else
      using default_eval_tag = Recursive;
    #endif

    // multi threading tags
    struct SingleThread;

    #if !defined(NURBS_THREAD_NUM)
      #define NURBS_THREAD_NUM 8
    #endif

    template <size_t N = NURBS_THREAD_NUM>
    struct MultiThread;

    #if defined(NURBS_MULTI_THREADING)
      using default_threading_tag = MultiThread<>;
    #else
      using default_threading_tag = SingleThread;
    #endif

  } // namespace tags
  namespace impl {
    template <class> struct is_mt_tag : std::false_type {};
    template <size_t N> struct is_mt_tag<tags::MultiThread<N>> : std::true_type {
      static constexpr size_t max_thread_num = N;
    };

    template <class T>
    static constexpr size_t max_thread_num_v = is_mt_tag<T>::max_thread_num;

    template <class T> static constexpr bool is_mt_tag_v = is_mt_tag<T>::value;
  } // namespace impl
}
