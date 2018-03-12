import Controller from '@ember/controller';
import Ember from 'ember';

export default Controller.extend({
  session: Ember.inject.service(),
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
        var data  = resp;
			  if(data.trim() == 'false')
			    that.set('message',"Enter your credentials correctly");
        else if(data.trim() == 'true'){
          /*that.get('session').login(that.get('id'),that.get('pass')).then(() => {
            that.transitionToRoute('/home');
          }, (err) => {
            alert('Error obtaining token: ' + err.responseText);
          });
        }
      }).catch(function(error){
        alert(error);
        that.transitionToRoute('/home');
      });*/
        that.get('session').login(that.get('id'),that.get('pass'));
        that.transitionToRoute('/home');
        }
      });
    }
  }
});
