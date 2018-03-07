import Controller from '@ember/controller';

export default Controller.extend({
  ajax: Ember.inject.service(),
  actions: {
    login() {
      alert("dsfsdf");
      alert(this.get('id'));
      alert(this.get('pass'));
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
        alert(resp);
      }).catch(function(error){
        alert("Some error");
      });
    }
  }
});
