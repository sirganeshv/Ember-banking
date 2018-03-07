import Controller from '@ember/controller';

export default Controller.extend({
  ajax: Ember.inject.service(),
  actions: {
    login() {
      var that  = this;
      Ember.$.ajax({
        url: "/login",
        type: "POST",
        data: {
            "ID" : this.get('id'),
            "password" : this.get('pass')
        }
      }).then(function(resp){
        /*var data = JSON.parse(resp);
        that.set('name',data.name);
        that.set('age',data.age);*/
        var data  = resp;
        if(data.trim() == 'false')
          that.set('message',"Enter your credentials correctly");
        else if(data.trim() == 'true'){
          that.transitionToRoute('/home');
        }
      }).catch(function(error){
        alert(JSON.stringify(error));
      });
    }
  }
});
