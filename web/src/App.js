// ============================================================================
// MAIN APPLICATION COMPONENT
// ============================================================================

import React, { useEffect } from 'react';
import Header from './components/Header';
import Downloads from './components/Downloads';
import Changelog from './components/Changelog';
import config from './config';
import './App.css';

/**
 * App Component
 * Root component that orchestrates the application layout
 */
function App() {
  // Set document title to plugin name
  useEffect(() => {
    document.title = `${config.pluginName} - ${config.companyName}`;
  }, []);

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

