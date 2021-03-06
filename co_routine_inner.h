/*
* Tencent is pleased to support the open source community by making Libco available.

* Copyright (C) 2014 THL A29 Limited, a Tencent company. All rights reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License"); 
* you may not use this file except in compliance with the License. 
* You may obtain a copy of the License at
*
*	http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, 
* software distributed under the License is distributed on an "AS IS" BASIS, 
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
* See the License for the specific language governing permissions and 
* limitations under the License.
*/


#ifndef __CO_ROUTINE_INNER_H__

#include "co_routine.h"
#include "coctx.h"
struct stCoRoutineEnv_t;
struct stCoSpec_t
{
	void *value;
};
//协程栈
struct stStackMem_t
{
	stCoRoutine_t* occupy_co;//当前占用“协程栈”的协程
	int stack_size;//栈的大小
	char* stack_bp; //stack_buffer + stack_size。栈的基址，栈是从高到低扩展
	char* stack_buffer;

};

//协程栈管理结构，共享栈
struct stShareStack_t
{
	unsigned int alloc_idx;
	int stack_size;
	int count;//协程栈数组中元素个数
	stStackMem_t** stack_array;//协程栈数组
};


//协程
struct stCoRoutine_t
{
	stCoRoutineEnv_t *env;//当前所在线程的环境变量，对于整个线程上的所有协程来说，它是全局性的
	pfn_co_routine_t pfn;//协程执行的主体函数
	void *arg;//主体函数的参数
	coctx_t ctx;//协程上下文

	char cStart;
	char cEnd;
	char cIsMain;
	char cEnableSysHook;
	char cIsShareStack;

	void *pvEnv;

	//char sRunStack[ 1024 * 128 ];
	stStackMem_t* stack_mem;


	//save satck buffer while confilct on same stack_buffer;
  //共享栈由于是环式的，所以 在分配新的栈时可能用的原来的内存，存在冲突，
  //这个时候需要先把原来内存里面的内容拷贝出来，进行保存，然后才能复用。
	char* stack_sp; 
	unsigned int save_size;
	char* save_buffer;

	stCoSpec_t aSpec[1024];

};



//1.env
void 				co_init_curr_thread_env();
stCoRoutineEnv_t *	co_get_curr_thread_env();

//2.coroutine
void    co_free( stCoRoutine_t * co );
void    co_yield_env(  stCoRoutineEnv_t *env );

//3.func



//-----------------------------------------------------------------------------------------------

struct stTimeout_t;
struct stTimeoutItem_t ;

stTimeout_t *AllocTimeout( int iSize );
void 	FreeTimeout( stTimeout_t *apTimeout );
int  	AddTimeout( stTimeout_t *apTimeout,stTimeoutItem_t *apItem ,uint64_t allNow );

struct stCoEpoll_t;
stCoEpoll_t * AllocEpoll();
void 		FreeEpoll( stCoEpoll_t *ctx );

stCoRoutine_t *		GetCurrThreadCo();
void 				SetEpoll( stCoRoutineEnv_t *env,stCoEpoll_t *ev );

typedef void (*pfnCoRoutineFunc_t)();

#endif

#define __CO_ROUTINE_INNER_H__
