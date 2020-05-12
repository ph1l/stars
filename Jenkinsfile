pipeline {
  agent any
  stages {
    stage('Clean') {
      steps { sh 'make clean' }
    }
    stage('Build') {
      steps { sh 'make all' }
    }
  }
  post {
    always {
      archiveArtifacts artifacts: '?stars', fingerprint: true
    }
  }
}
