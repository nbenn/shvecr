
#' @importFrom shmemr new_mem
#' @export
new_vec3 <- function(length, data_type = 14, ...) {

  mem <- new_mem(0, ...)

  res <- create_vec3(mem, length, 14)
  attr(res, "mem") <- mem

  res
}
