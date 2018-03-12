import Controller from '@ember/controller';

export default Controller.extend({
  session: Ember.inject.service(),
  ajax: Ember.inject.service(),
  actions: {
    add_customer() {
      var that  = this;
      Ember.$.ajax({
        url: "/add_customer",
        type: "POST",
        data: {
            "name" : this.get('name'),
            "age" : this.get('age'),
            "phone" : this.get('phone'),
            "address" : this.get('address'),
            "passphrase" : this.get('passphrase'),
            "security_qn" : this.get('security_qn'),
            "security_ans" : this.get('security_ans'),
            "operator_id" : this.get('session').currentUser,
        }
      }).then(function(resp){
        alert(resp);
        });
    }
  }
});
