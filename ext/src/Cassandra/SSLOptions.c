#include "php_cassandra.h"

zend_class_entry *cassandra_ssl_options_ce = NULL;

static zend_function_entry cassandra_ssl_options_methods[] = {
  PHP_FE_END
};

static zend_object_handlers cassandra_ssl_options_handlers;

static HashTable*
php_cassandra_ssl_options_properties(zval *object TSRMLS_DC)
{
  cassandra_ssl_options* options = (cassandra_ssl_options*) zend_object_store_get_object(object TSRMLS_CC);
  HashTable*             props   = zend_std_get_properties(object TSRMLS_CC);

  return props;
}

static int
php_cassandra_ssl_options_compare(zval *obj1, zval *obj2 TSRMLS_DC)
{
  if (Z_OBJCE_P(obj1) != Z_OBJCE_P(obj2))
    return 1; /* different classes */

  return Z_OBJ_HANDLE_P(obj1) != Z_OBJ_HANDLE_P(obj1);
}

static void
php_cassandra_ssl_options_free(void *object TSRMLS_DC)
{
  cassandra_ssl_options* options = (cassandra_ssl_options*) object;

  zend_object_std_dtor(&options->zval TSRMLS_CC);
  cass_ssl_free(options->ssl);
  efree(options);
}

static zend_object_value
php_cassandra_ssl_options_new(zend_class_entry* class_type TSRMLS_DC)
{
  zend_object_value retval;
  cassandra_ssl_options *options;

  options = (cassandra_ssl_options*) ecalloc(1, sizeof(cassandra_ssl_options));

  zend_object_std_init(&options->zval, class_type TSRMLS_CC);
  object_properties_init(&options->zval, class_type);

  options->ssl = cass_ssl_new();

  retval.handle   = zend_objects_store_put(options,
                      (zend_objects_store_dtor_t) zend_objects_destroy_object,
                      php_cassandra_ssl_options_free, NULL TSRMLS_CC);
  retval.handlers = &cassandra_ssl_options_handlers;

  return retval;
}

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(SSLOptions)
{
  zend_class_entry ce;

  INIT_CLASS_ENTRY(ce, "Cassandra\\SSLOptions", cassandra_ssl_options_methods);
  cassandra_ssl_options_ce = zend_register_internal_class(&ce TSRMLS_CC);
  cassandra_ssl_options_ce->ce_flags     |= ZEND_ACC_FINAL_CLASS;
  cassandra_ssl_options_ce->create_object = php_cassandra_ssl_options_new;

  memcpy(&cassandra_ssl_options_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
  cassandra_ssl_options_handlers.get_properties  = php_cassandra_ssl_options_properties;
  cassandra_ssl_options_handlers.compare_objects = php_cassandra_ssl_options_compare;

  return SUCCESS;
}