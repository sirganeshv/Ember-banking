import Controller from '@ember/controller';
import Ember from 'ember';

export default Controller.extend({
  session: Ember.inject.service(),
  actions: {
    logout() {
      alert("logged out ");
      this.get('session').logout();
      this.transitionToRoute('/login');
    }
  }
});
