/*
 * Distributed under OSI-approved BSD 3-Clause License. See
 * accompanying License.txt
 */

#ifndef catalyst_dump_node_h
#define catalyst_dump_node_h

#ifdef CATALYST_USE_MPI
#include "mpi.h"
#endif

#include <conduit_utils.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32) && !defined(__CYGWIN__)
#define PATH_SEP '\\'
#else
#define PATH_SEP '/'
#endif

void get_mpi_info(int& rank, int& num_ranks)
{
  rank = 0;
  num_ranks = 1;
#ifdef CATALYST_USE_MPI
  int mpi_initialized = 0;
  MPI_Initialized(&mpi_initialized);
  if (mpi_initialized)
  {
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);
  }
#endif
}

// Constructs the absolute path of the file to write out.
// Similar to python's os.path.join.
char* construct_full_path(
  const char* out_dir, const char* stage, unsigned long invocations, int use_invocations)
{
  int num_ranks = 1;
  int rank = 0;
  get_mpi_info(rank, num_ranks);

  size_t out_dir_len = strlen(out_dir);
  if (!out_dir_len)
  {
    fprintf(stderr, "Empty data_dump_directory detected.\n");
    exit(1);
  }

  // Add a separator to the end of the directory if needed.
  char sep_or_no_sep[2];
  memset(sep_or_no_sep, 0, 2);
  if (out_dir[out_dir_len - 1] != PATH_SEP)
  {
    sep_or_no_sep[0] = PATH_SEP;
  }

  size_t full_path_len = 0;
  char* full_path = NULL;

  // If we want to include the invocation number in the filename,
  // use a different format for the filename
  if (use_invocations)
  {
    // Determine the length of the path
    full_path_len = snprintf(NULL, 0, "%s%s%s_invc%lu_params.conduit_bin.%d.%d", out_dir,
      sep_or_no_sep, stage, invocations, num_ranks, rank);

    // Format the string with the correct information
    full_path = (char*)calloc(full_path_len + 1, sizeof(char));
    snprintf(full_path, full_path_len + 1, "%s%s%s_invc%lu_params.conduit_bin.%d.%d", out_dir,
      sep_or_no_sep, stage, invocations, num_ranks, rank);
  }
  else
  {
    // Determine the length of the path
    full_path_len = snprintf(
      NULL, 0, "%s%s%s_params.conduit_bin.%d.%d", out_dir, sep_or_no_sep, stage, num_ranks, rank);

    // Format the string with the correct information
    full_path = (char*)calloc(full_path_len + 1, sizeof(char));
    snprintf(full_path, full_path_len + 1, "%s%s%s_params.conduit_bin.%d.%d", out_dir,
      sep_or_no_sep, stage, num_ranks, rank);
  }

  if (!(full_path_len && full_path))
  {
    fprintf(stderr, "Invalid dump_node filename convention.\n");
    exit(1);
  }

  return full_path;
}

bool ensure_directory(const char* out_dir)
{
  int rank = 0;
  int num_ranks = 1;
  get_mpi_info(rank, num_ranks);

  int directory_exists = 0;
  if (rank == 0)
  {
    directory_exists =
      (conduit::utils::is_directory(out_dir) || conduit::utils::create_directory(out_dir)) ? 1 : 0;
  }
#ifdef CATALYST_USE_MPI
  if (num_ranks > 1)
  {
    MPI_Bcast(&directory_exists, 1, MPI_INT, 0, MPI_COMM_WORLD);
  }
#endif
  return (directory_exists == 1);
}

// Check if a data dump directory has been set. If it has,
// write out the params argument to disk to load later.
void dump_node(
  const conduit_node* params, const char* stage, unsigned long invocations, int use_invocations)
{
  const char* out_dir = getenv("CATALYST_DATA_DUMP_DIRECTORY");
  if (out_dir && ensure_directory(out_dir))
  {
    char* full_path = construct_full_path(out_dir, stage, invocations, use_invocations);
    conduit_node_save((conduit_node*)(params), full_path, "conduit_bin");
    free(full_path);
  }
}

#endif
