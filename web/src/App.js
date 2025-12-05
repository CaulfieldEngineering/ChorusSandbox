// ============================================================================
// MAIN APPLICATION COMPONENT
// ============================================================================

import React from 'react';
import Header from './components/Header';
import Downloads from './components/Downloads';
import Changelog from './components/Changelog';
import './App.css';

/**
 * App Component
 * Root component that orchestrates the application layout
 */
function App() {
  return (
    <div className="app">
      <main className="app__main">
        <div className="app__content">
          <Header />
          <Downloads />
          <Changelog />
        </div>
      </main>
    </div>
  );
}

export default App;

