import Service from '@ember/service';
import Ember from 'ember';

export default Ember.Service.extend({
  currentUser: null,
  store: Ember.inject.service(),
  login(userId,password){
      this.set("currentUser", userId);
      Cookies.set('currentUser', userId);
      //alert("cookie is " + Cookies.get('currentUser'));
  },
  logout(){
    this.set("currentUser", null);
    Cookies.remove('currentUser');
  },
  initializeFromCookie: function(){
    var userId = Cookies.get('currentUser');
    this.set('currentUser',userId);
    /*if(!!userId){
      var userId = this.get('store').find('currentUser', userId);
      this.set('currentUser', userId);
    }*/
  }.on('init')
});
