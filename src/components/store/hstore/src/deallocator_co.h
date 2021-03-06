/*
   Copyright [2017-2019] [IBM Corporation]
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
       http://www.apache.org/licenses/LICENSE-2.0
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/


#ifndef COMANCHE_HSTORE_DEALLOCATOR_CO_H
#define COMANCHE_HSTORE_DEALLOCATOR_CO_H

#include "heap_co.h"
#include "hop_hash_log_.h"
#include "persister_pmem.h"
#include "pointer_pobj.h"
#include "store_root.h"
#include "trace_flags.h"

#pragma GCC diagnostic push
#if defined __clang__
#pragma GCC diagnostic ignored "-Wnested-anon-types"
#endif
#pragma GCC diagnostic ignored "-Wold-style-cast"
#include <libpmemobj.h> /* pmemobj_free */
#pragma GCC diagnostic pop

#include <cstdlib> /* size_t, ptrdiff_t */

template <typename T, typename Persister = persister_pmem>
	class deallocator_co;

template <>
	class deallocator_co<void, persister_pmem>
		: public persister_pmem
	{
	public:
		using pointer = pointer_pobj<void, 0U>;
		using const_pointer = pointer_pobj<const void, 0U>;
		using value_type = void;
		template <typename U, typename P = persister_pmem>
			struct rebind
			{
				using other = deallocator_co<U, P>;
			};
	};

template <class Persister>
	class deallocator_co<void, Persister>
		: public Persister
	{
	public:
		using pointer = pointer_pobj<void, 0U>;
		using const_pointer = pointer_pobj<const void, 0U>;
		using value_type = void;
		template <typename U, typename P = Persister>
			struct rebind
			{
				using other = deallocator_co<U, P>;
			};
	};

template <typename T, typename Persister>
	class deallocator_co
		: public Persister
	{
	public:
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
		using pointer = pointer_pobj<T, 0U>;
		using const_pointer = pointer_pobj<const T, 0U>;
		using reference = T &;
		using const_reference = const T &;
		using value_type = T;

		template <typename U>
			struct rebind
			{
				using other = deallocator_co<U, Persister>;
			};

		deallocator_co(const Persister = Persister()) noexcept
		{}

		deallocator_co(const deallocator_co &) noexcept = default;

		template <typename U>
			deallocator_co(const deallocator_co<U, Persister> &) noexcept
				: deallocator_co()
			{}

		deallocator_co &operator=(const deallocator_co &) = delete;

		pointer address(reference x) const noexcept /* to be deprecated */
		{
			return pointer(pmemobj_oid(&x));
		}
		const_pointer address(const_reference x) const noexcept /* to be deprecated */
		{
			return pointer(pmemobj_oid(&x));
		}

		void deallocate(
			pointer ptr
			, size_type sz_
		)
		{
			auto pool = ::pmemobj_pool_by_oid(ptr);

			TOID_DECLARE_ROOT(struct store_root_t);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
			TOID(struct store_root_t) root = POBJ_ROOT(pool, struct store_root_t);
			assert(!TOID_IS_NULL(root));
#pragma GCC diagnostic ignored "-Wpedantic"
			auto heap = static_cast<heap_co *>(pmemobj_direct((D_RO(root)->heap_oid)));
#pragma GCC diagnostic pop
#if TRACE_PALLOC
			{
				auto ptr = static_cast<char *>(pmemobj_direct(oid));
				hop_hash_log::write(__func__
					, " [", ptr
					, "..", static_cast<void *>(ptr + s * sizeof(T))
					, ")"
				);
			}
#endif
			heap->free(ptr, sizeof(T) * sz_);
		}
		auto max_size() const
		{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
			return PMEMOBJ_MAX_ALLOC_SIZE;
#pragma GCC diagnostic pop
		}
		void persist(const void *ptr, size_type len, const char * = nullptr)
		{
			Persister::persist(ptr, len);
		}
	};

#endif
