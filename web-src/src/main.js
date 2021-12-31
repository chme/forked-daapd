import { createApp } from 'vue'
import store from './store'
import { router } from './router'
import VueProgressBar from '@aacassandra/vue3-progressbar'
import { ObserveVisibility } from 'vue-observe-visibility'
import App from './App.vue'

createApp(App)
  .use(store)
  .use(router)
  .use(VueProgressBar, {
    color: 'hsl(204, 86%, 53%)',
    failedColor: 'red',
    height: '1px'
  })
  .directive('observe-visibility', {
    beforeMount: ObserveVisibility.beforeMount,
    update: ObserveVisibility.update,
    unmounted: ObserveVisibility.unbind
  })
  .mount('#app')
