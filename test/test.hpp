#pragma once

#define t_assert(M,C) nurbs::test::_assert(std::string(M)+std::string("\n| File: " __FILE__ "\n| Line: " TOSTRING(__LINE__)),C);
#define t_test(M,C) nurbs::test::_test(std::string(M) + std::string("\n| File: " __FILE__ "\n| Line: " TOSTRING(__LINE__)),C);
#define TOSTRING(n) TOSTRING_(n)
#define TOSTRING_(n) #n

#include <string>
#include <vector>
#include <iostream>
namespace nurbs{
  struct test{
    static int test_run;
    static int test_failed;
    static std::string test_name;
    static std::vector<std::string> messages;
    static void _assert(std::string message, bool cond){
      if(cond){
        test_run++;
      }else{
        test_failed++;
        messages.push_back(std::string("\033[1;31m!! t_assert failed !!\033[0m\n")+message);
        summarize();
        std::exit(test_failed);
      }
    }
    static void _test(std::string message,bool cond){
      if(cond){
        test_run++;
      }else{
        test_failed++;
        messages.push_back(message);
      }
    }

    static void summarize(){
      std::cout << "Summary of nurbs::test" << std::endl
                << " Test name: " << test_name << std::endl
                << " Total test run:    " << test_run  << std::endl
                << " Total test failed: " << test_failed << std::endl
                << std::endl;

      if(test_failed){
        std::cout << " All of error massages:" << std::endl;
        for (auto str : messages)
          std::cout << str << std::endl;
      }

    }
  };
  int test::test_run=0;
  int test::test_failed=0;
  std::string test::test_name = "";
  std::vector<std::string> test::messages= {};
}
