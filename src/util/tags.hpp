#pragma once

// Tags
namespace nurbs::tags {
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

} // namespace nurbs::tags
