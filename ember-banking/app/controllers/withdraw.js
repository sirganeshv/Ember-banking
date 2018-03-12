import Controller from '@ember/controller';

export default Controller.extend({
  session: Ember.inject.service(),
  ajax: Ember.inject.service(),
  actions: {
    withdraw() {
      var that  = this;
      Ember.$.ajax({
        url: "/withdraw",
        type: "POST",
        data: {
            "acc_no" : this.get('acc_no'),
            "money" : this.get('money'),
            "customer_passphrase" : this.get("customer_passphrase"),
            "operator_id" : this.get('session').currentUser,
            "operator_password" : this.get("operator_password")
        }
      }).then(function(resp){
        alert(resp);
        });
    }
  }
});
