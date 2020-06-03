#ifndef BERT_CLEANUP_H
#define BERT_CLEANUP_H

#define on_destroy(T, _cleanup)  __attribute__((cleanup(_cleanup))) T

#endif
