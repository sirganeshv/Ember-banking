import EmberRouter from '@ember/routing/router';
import config from './config/environment';

const Router = EmberRouter.extend({
  location: config.locationType,
  rootURL: config.rootURL
});

Router.map(function() {
  this.route('login');
  this.route('home');
  this.route('add_operator');
  this.route('add_customer');
  this.route('update_customer');
  this.route('delete_account');
  this.route('deposit');
  this.route('withdraw');
  this.route('transfer');
});

export default Router;
