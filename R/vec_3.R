
#' @importFrom shmemr new_mem
#' @export
new_vec3 <- function(length, data_type = double(), ...) {

  mem <- new_mem(0, ...)

  res <- create_vec3(mem, length, data_type)
  attr(res, "mem") <- mem

  structure(res, class = "vec3")
}
