import Controller from '@ember/controller';

export default Controller.extend({
  session: Ember.inject.service(),
  ajax: Ember.inject.service(),
  actions: {
    delete() {
      Ember.$.ajax({
        url: "/delete_account",
        type: "POST",
        data: {
            "acc_no" : this.get('acc_no'),
            "operator_id" : this.get('session').currentUser,
            "operator_password" : this.get('operator_password'),
        }
      }).then(function(resp){
        alert(resp);
        });
    }
  }
});
