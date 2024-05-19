EXTENSION = pg_unbreak        # the extensions name
DATA = pg_unbreak--0.0.1.sql  # script files to install
MODULES = pg_unbreak

# postgres build stuff
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)