-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION pg_unbreak" to load this file. \quit

CREATE FUNCTION repair() RETURNS INT
    AS 'MODULE_PATHNAME', 'repair'
    LANGUAGE C STRICT;