#include "ngram_tokenizer.h"
#include <cstring>
#include <sqlite3ext.h>

SQLITE_EXTENSION_INIT1;

/*
** Return a pointer to the fts5_api pointer for database connection db.
** If an error occurs, return NULL and leave an error in the database
** handle (accessible using sqlite3_errcode()/errmsg()).
*/
fts5_api *fts5_api_from_db(sqlite3 *db) {
  fts5_api *pRet = 0;
  sqlite3_stmt *pStmt = 0;

  if (SQLITE_OK == sqlite3_prepare(db, "SELECT fts5(?1)", -1, &pStmt, 0)) {
    sqlite3_bind_pointer(pStmt, 1, (void *)&pRet, "fts5_api_ptr", nullptr);
    sqlite3_step(pStmt);
  }
  sqlite3_finalize(pStmt);
  return pRet;
}

extern "C" {
int sqlite3_ngram_xCreate(void *, const char **azArg, int nArg,
                          Fts5Tokenizer **ppOut) {
  int n = 1;

  for (int i = 0; i < nArg; i++) {
    if (std::strncmp(azArg[i], "n", 2) == 0 && i + 1 < nArg) {
      n = std::strtol(azArg[i + 1], nullptr, 10);
      break;
    }
  }

  *ppOut = reinterpret_cast<Fts5Tokenizer *>(new NgramTokenizer(n));
  return SQLITE_OK;
}

void sqlite3_ngram_xDelete(Fts5Tokenizer *handler) {
  NgramTokenizer *tokenizer = reinterpret_cast<NgramTokenizer *>(handler);
  delete tokenizer;
}

int sqlite3_ngram_xTokenize(
    Fts5Tokenizer *handler, void *pCtx,
    int flags,                        /* Mask of FTS5_TOKENIZE_* flags */
    const char *pText, int nText,
    int (*xToken)(void *pCtx,         /* Copy of 2nd argument to xTokenize() */
                  int tflags,         /* Mask of FTS5_TOKEN_* flags */
                  const char *pToken, /* Pointer to buffer containing token */
                  int nToken,         /* Size of token in bytes */
                  int iStart,         /* Byte offset of token within input text */
                  int iEnd            /* Byte offset of end of token within input text */
                  )) {
  NgramTokenizer *tokenizer = reinterpret_cast<NgramTokenizer *>(handler);
  int rc = tokenizer->tokenize(pText, nText,
                               [&](const char *token, const int &token_len,
                                   const int &start, const int &end) -> int {
                                 return xToken(pCtx, 0, token, token_len, start,
                                               end);
                               });

  return rc;
}

int sqlite3_ngram_init(sqlite3 *db, char **pzErrMsg,
                       const sqlite3_api_routines *pApi) {
  int rc = SQLITE_OK;
  SQLITE_EXTENSION_INIT2(pApi);
  /* insert code to initialize your extension here */
  fts5_api *api = fts5_api_from_db(db);
  if (api == nullptr) {
    return SQLITE_ERROR;
  }

  fts5_tokenizer tokenizer{
      .xCreate = sqlite3_ngram_xCreate,
      .xDelete = sqlite3_ngram_xDelete,
      .xTokenize = sqlite3_ngram_xTokenize,
  };
  rc = api->xCreateTokenizer(api, "ngram", nullptr, &tokenizer, nullptr);
  if (rc) {
    return rc;
  }

  return rc;
}

}
