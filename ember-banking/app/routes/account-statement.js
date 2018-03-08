import Route from '@ember/routing/route';

export default Route.extend({
  ajax: Ember.inject.service(),
  actions: {
    display() {
      var that  = this;
      Ember.$.ajax({
        url: "/account_statement",
        type: "POST",
        data: {
          "acc_no" : $('#acc_no').val()
        }
      }).then(function(resp){
        alert(resp);
      }).catch(function(error){
        alert(JSON.stringify(error));
      });
    }
  }
});
