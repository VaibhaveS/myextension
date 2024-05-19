#include "postgres.h"
#include "fmgr.h"
#include "executor/spi.h"
#include "utils/builtins.h"

PG_MODULE_MAGIC;

void
_PG_init(void)
{
    SPI_connect();
    const char *create_table_cmd = 
        "CREATE TABLE IF NOT EXISTS pg_unbreak_config ( \
            id serial PRIMARY KEY, \
            name text NOT NULL, \
            replace_with text NOT NULL); \
        ";

    SPI_execute(create_table_cmd, false, 0);
    SPI_finish();
}

PG_FUNCTION_INFO_V1(repair);

Datum
repair(PG_FUNCTION_ARGS)
{
    SPI_connect();
    const char *fetch_config_cmd = 
        "SELECT name, replace_with from pg_unbreak_config";
        
    SPI_execute(fetch_config_cmd, true, 0);

    if (SPI_tuptable == NULL) {
        elog(INFO, "This is an informational message gaja");
        SPI_finish();
        PG_RETURN_INT32(0);
    }

    int repaired = 0;
    HeapTuple* tuple = SPI_tuptable->vals;
    TupleDesc tupdesc = SPI_tuptable->tupdesc;
    for(; tuple != NULL; tuple++) {
        char *name = SPI_getvalue(*tuple, tupdesc, 1);
        char *replace_with = SPI_getvalue(*tuple, tupdesc, 2);

        const char *fetch_columns_cmd = 
            "SELECT table_name, column_name "
            "FROM information_schema.columns "
            "WHERE data_type = 'name' "
            "AND table_catalog = current_database() "
            "AND column_name = $1";

        Oid argtypes[] = { TEXTOID };
        Datum values[] = { CStringGetTextDatum(name) };
        repaired += SPI_execute_with_args(fetch_columns_cmd, 1, argtypes, values, NULL, false, 0);
    }

    elog(INFO, "This is an informational message %d", repaired);

    SPI_finish();
    PG_RETURN_INT32(repaired);
}