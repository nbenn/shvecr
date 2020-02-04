
#' @importFrom shmemr new_mem
#' @export
new_vec3 <- function(length, data_type = double(), ...) {

  mem <- new_mem(0, ...)

  res <- create_vec3(mem, length, data_type)
  attr(res, "mem") <- mem

  structure(res, class = "vec3")
}

#' @export
new_altvec <- function(length, data_type = double(),
                       name = shmemr::rand_name(),
                       type = shmemr::list_mem_types()) {

  assert_that(identical(typeof(data_type), "double"))

  type <- match.arg(type)

  res <- new_altreal(name, length, type)

  structure(res, class = "altvec")
}
