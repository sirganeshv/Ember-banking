import Controller from '@ember/controller';

export default Controller.extend({
  session: Ember.inject.service(),
  ajax: Ember.inject.service(),
  actions: {
    schedule_transfer() {
      var that  = this;
      Ember.$.ajax({
        url: "/schedule_transfer",
        type: "POST",
        data: {
            "withdraw_acc_no" : this.get('withdraw_acc_no'),
            "acc_no" : this.get('acc_no'),
            "money" : this.get('money'),
            "customer_passphrase" : this.get("customer_passphrase"),
            "operator_id" : this.get('session').currentUser,
            "operator_password" : this.get('operator_password'),
            "hour" : this.get('hour'),
            "min" : this.get('min')
        }
      }).then(function(resp){
        alert(resp);
        });
    }
  }
});
