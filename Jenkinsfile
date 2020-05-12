pipeline {
  agent any
  stages {
    stage('Clean') {
      steps { sh 'make clean' }
    }
    stage('Build') {
      steps { sh 'make all' }
    }
    stage('Check') {
      steps { sh 'make check || true' }
    }
  }
  post {
    always {
      recordIssues enabledForFailure: true, tool: gcc(), qualityGates: [[threshold: 1, type: 'TOTAL', unstable: true]]
      step([$class: 'XUnitPublisher', thresholdMode: 1,
        thresholds: [
          [$class: 'FailedThreshold', unstableThreshold: '1'],
          [$class: 'SkippedThreshold', unstableThreshold: '1']
        ],
        tools: [[
          $class: 'CUnitJunitHudsonTestType',
          deleteOutputFiles: true,
          failIfNotNew: true,
          pattern: 'test_results/*-Results.xml',
          skipNoTestFiles: false,
          stopProcessingIfError: true
        ]]
      ])
      archiveArtifacts artifacts: '?stars', fingerprint: true
    }
  }
}
