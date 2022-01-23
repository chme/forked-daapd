# Vue 3 + Vite Migration

- Vue Dev Tools required in version 6 (currently only released as beta versions): <https://devtools.vuejs.org/guide/installation.html#beta>

- [ ] `vue-range-slider` is not compatible with vue3
  - replacement option: <https://github.com/vueform/slider>
  - used `@vueform/slider` for volume control
  - [x] track progress (now playing)
  - [ ] track progress (podcasts)

- [x] vue-router does not support navigation guards in mixins: <https://github.com/vuejs/vue-router-next/issues/454>
  - replace mixin with composition api? <https://next.router.vuejs.org/guide/advanced/composition-api.html#navigation-guards>
  - Copied nav guards into each component

- [x] vue-router link does not support `tag` and `active-class` properties: <https://next.router.vuejs.org/guide/migration/index.html#removal-of-event-and-tag-props-in-router-link>

- [ ] vite does not support env vars in `vite.config.js` from `.env` files
  - <https://stackoverflow.com/questions/66389043/how-can-i-use-vite-env-variables-in-vite-config-js>
  - <https://github.com/vitejs/vite/issues/1930>

- [x] `vue-tiny-lazyload-img` does not support Vue 3
  - No sign of interesst to add support <https://github.com/mazipan/vue-tiny-lazyload-img>
  - `v-lazy-image` (<https://github.com/alexjoverm/v-lazy-image>) seems to be supported and popular
    - Works as a component instead of a directive
    - __DOES NOT__ have a good error handling, if the (remote) image does not exist
  - `vue3-lazyload` (<https://github.com/murongg/vue3-lazyload>)
    - Works as a directive
    - Easy replacement for `vue-tiny-lazyload-img`

- [ ] Top margin in pages is wrong (maybe related to vue-router scroll behavior changes?)

- [x] Mobile view seems to be broken
  - Looks like the cause of this was the broken router-link in bulma tabs component

- [x] Changing sort option (artist albums view) does not work

- [ ] Replace unmaintained `vue-infinite-loading` dependency

- [x] Replace `bulma-switch` with `@vueform/toggle`?
  - Update of `bulma-switch` (or `vite`) fixed the import of the sass file, no need to replace it now

- [ ] Add linting (ESLint) ?

- [ ] Check transitions
