// Copyright (C) 2019  Nicolas Bennett

// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.

// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
// more details.

// You should have received a copy of the GNU General Public License along
// with this program. If not, see <http://www.gnu.org/licenses/>.

#include <shmemr.h>

#include <Rinternals.h>
#include <R_ext/Rallocators.h>

void* my_alloc(R_allocator_t *allocator, size_t size)
{
  auto mem = (SEXP)allocator->data;
  shmemr::mem_resize(mem, static_cast<double>(size));
  return R_ExternalPtrAddr(shmemr::get_mem_ptr(mem));
}

void my_free(R_allocator_t *allocator, void* addr) { }

// [[Rcpp::export]]
SEXP create_vec3(SEXP mem, double length, SEXP type) {

  auto ator = (R_allocator_t*)malloc(sizeof(R_allocator_t));
  ator->mem_alloc = &my_alloc;
  ator->mem_free = &my_free;
  ator->res = NULL;
  ator->data = mem;

  return Rf_allocVector3(TYPEOF(type), static_cast<R_xlen_t>(length), ator);
}
