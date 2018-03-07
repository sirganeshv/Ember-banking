/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class Banking */

#ifndef _Included_Banking
#define _Included_Banking
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     Banking
 * Method:    get_acc_no
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_Banking_get_1acc_1no
  (JNIEnv *, jobject);

/*
 * Class:     Banking
 * Method:    get_customer_from_list
 * Signature: (I)LCustomer;
 */
JNIEXPORT jobject JNICALL Java_Banking_get_1customer_1from_1list
  (JNIEnv *, jobject, jint);

/*
 * Class:     Banking
 * Method:    read_customer
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_Banking_read_1customer
  (JNIEnv *, jobject, jint);

/*
 * Class:     Banking
 * Method:    find_customer_position
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_Banking_find_1customer_1position
  (JNIEnv *, jobject, jint);

/*
 * Class:     Banking
 * Method:    is_customer_under_current_operator
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_Banking_is_1customer_1under_1current_1operator
  (JNIEnv *, jobject, jint);

/*
 * Class:     Banking
 * Method:    is_current_operator_admin
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_Banking_is_1current_1operator_1admin
  (JNIEnv *, jobject);

/*
 * Class:     Banking
 * Method:    add_operator
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Banking_add_1operator
  (JNIEnv *, jobject);

/*
 * Class:     Banking
 * Method:    initialize
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Banking_initialize
  (JNIEnv *, jobject);

/*
 * Class:     Banking
 * Method:    add_account
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Banking_add_1account
  (JNIEnv *, jobject);

/*
 * Class:     Banking
 * Method:    update_account
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Banking_update_1account
  (JNIEnv *, jobject);

/*
 * Class:     Banking
 * Method:    delete_account
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Banking_delete_1account
  (JNIEnv *, jobject);

/*
 * Class:     Banking
 * Method:    display_customer
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_Banking_display_1customer
  (JNIEnv *, jobject, jint);

/*
 * Class:     Banking
 * Method:    display_customer_list
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Banking_display_1customer_1list
  (JNIEnv *, jobject);

/*
 * Class:     Banking
 * Method:    deposit
 * Signature: (II)Z
 */
JNIEXPORT jboolean JNICALL Java_Banking_deposit
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     Banking
 * Method:    record_deposit
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_Banking_record_1deposit
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     Banking
 * Method:    withdraw_money
 * Signature: (II)Z
 */
JNIEXPORT jboolean JNICALL Java_Banking_withdraw_1money
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     Banking
 * Method:    record_withdrawal
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_Banking_record_1withdrawal
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     Banking
 * Method:    transfer_money
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Banking_transfer_1money
  (JNIEnv *, jobject);

/*
 * Class:     Banking
 * Method:    print_account_statement
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Banking_print_1account_1statement
  (JNIEnv *, jobject);

/*
 * Class:     Banking
 * Method:    print_account_summary_in_range
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Banking_print_1account_1summary_1in_1range
  (JNIEnv *, jobject);

/*
 * Class:     Banking
 * Method:    schedule_transfer
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Banking_schedule_1transfer
  (JNIEnv *, jobject);

/*
 * Class:     Banking
 * Method:    write_files
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Banking_write_1files
  (JNIEnv *, jobject);

/*
 * Class:     Banking
 * Method:    print_pending_transactions
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Banking_print_1pending_1transactions
  (JNIEnv *, jobject);

/*
 * Class:     Banking
 * Method:    push_back
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_Banking_push_1back
  (JNIEnv *, jobject, jint);

/*
 * Class:     Banking
 * Method:    display_current_cache
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Banking_display_1current_1cache
  (JNIEnv *, jobject);

/*
 * Class:     Banking
 * Method:    get_cache_size
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_Banking_get_1cache_1size
  (JNIEnv *, jobject);

/*
 * Class:     Banking
 * Method:    clear_cache
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Banking_clear_1cache
  (JNIEnv *, jobject);

/*
 * Class:     Banking
 * Method:    add_standing_transactions
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Banking_add_1standing_1transactions
  (JNIEnv *, jobject);

/*
 * Class:     Banking
 * Method:    fixed_deposit
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Banking_fixed_1deposit
  (JNIEnv *, jobject);

/*
 * Class:     Banking
 * Method:    forgot_password
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_Banking_forgot_1password
  (JNIEnv *, jobject, jint);

#ifdef __cplusplus
}
#endif
#endif
